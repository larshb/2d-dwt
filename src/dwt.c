#include <stdio.h>
#include <malloc.h>

/* 5-3 2D Discrete Wavelet Transform*/

#define S(i) a[(i)*2]
#define D(i) a[(1+(i)*2)]
#define S_(i) ((i)<0?S(0):((i)>=sn?S(sn-1):S(i)))
#define D_(i) ((i)<0?D(0):((i)>=dn?D(dn-1):D(i)))

static void dwt_1d(int *a, int dn, int sn)
{
	int i;

	for (i = 0; i < dn; i++) {
		D(i) -= (S_(i) + S_(i + 1)) >> 1;
	}
	for (i = 0; i < sn; i++) {
		S(i) += (D_(i - 1) + D_(i) + 2) >> 2;
	}
}

static void deinterleave_h(int *a, int *b, int dn,
	int sn)
{
	int i;
	int * l_dest = b;
	int * l_src = a;

	for (i = 0; i < sn; ++i) {
		*l_dest++ = *l_src;
		l_src += 2;
	}

	l_dest = b + sn;
	l_src = a + 1;

	for (i = 0; i < dn; ++i) {
		*l_dest++ = *l_src;
		l_src += 2;
	}
}

static void deinterleave_v(int *a, int *b, int dn,
	int sn, int x)
{
	int i = sn;
	int * l_dest = b;
	int * l_src = a;

	while (i--) {
		*l_dest = *l_src;
		l_dest += x;
		l_src += 2;
	} /* b[i*x]=a[2*i+cas]; */

	l_dest = b + (size_t)sn * (size_t)x;
	l_src = a + 1;

	i = dn;
	while (i--) {
		*l_dest = *l_src;
		l_dest += x;
		l_src += 2;
	} /*b[(sn+i)*x]=a[(2*i+1-cas)];*/
}

/********************** 2D Section **********************/

//Hardcoded parameters
#define WIDTH 512
#define HEIGHT WIDTH

typedef struct {
	int x0, x1, y0, y1;
} resolution_t;

void dwt_2d(int* data, int resolutions)
{
	int i, j, k;
	int *a = 00;
	int *aj = 00;
	int *bj = 00;
	int w, l;

	int rw;           /* width of the resolution level computed   */
	int rh;           /* height of the resolution level computed  */
	size_t l_data_size;

	resolution_t l_cur_res = { .x0 = 0, .y0 = 0, .x1 = WIDTH, .y1 = HEIGHT };
	resolution_t l_last_res = { .x0 = 0, .y0 = 0, .x1 = WIDTH >> 1, .y1 = HEIGHT >> 1 };

	w = WIDTH;
	l = resolutions - 1;
	a = data;

	l_data_size = WIDTH;

	l_data_size *= sizeof(int);
	bj = (int*)malloc(l_data_size);

	i = l;

	while (i--) {
		int rw1;      /* width of the resolution level once lower than computed one                                       */
		int rh1;      /* height of the resolution level once lower than computed one                                      */
		int dn, sn;

		rw = l_cur_res.x1 - l_cur_res.x0;
		rh = l_cur_res.y1 - l_cur_res.y0;
		rw1 = l_last_res.x1 - l_last_res.x0;
		rh1 = l_last_res.y1 - l_last_res.y0;

		sn = rh1;
		dn = rh - rh1;
		for (j = 0; j < rw; ++j) {
			aj = a + j;
			for (k = 0; k < rh; ++k) {
				bj[k] = aj[k * w];
			}

			dwt_1d(bj, dn, sn);

			deinterleave_v(bj, aj, dn, sn, w);
		}

		sn = rw1;
		dn = rw - rw1;

		for (j = 0; j < rh; j++) {
			aj = a + j * w;
			for (k = 0; k < rw; k++) {
				bj[k] = aj[k];
			}
			dwt_1d(bj, dn, sn);
			deinterleave_h(bj, aj, dn, sn);
		}

		l_cur_res = l_last_res;
		l_last_res.x1 >>= 1;
		l_last_res.y1 >>= 1;
	}
}
