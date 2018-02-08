function run_test()

    % Parameters
    FILE        = 'res/lena.tif';
    RESOLUTIONS = 3;
    TOGRAYSCALE = true;
    UNBIASED    = true;

    % Read file
    raw = imread(FILE);
    if TOGRAYSCALE
        raw = rgb2gray(raw);
    end

    % Transform
    if ~UNBIASED
        raw = raw-128;
    end
    wav = zeros(size(raw));
    for i = 1:size(raw,3)
        wav(:,:,i) = dwt2_exe(raw(:,:,i), RESOLUTIONS);
    end

    % Compare
    imshowpair(raw, uint8(wav+128*(~UNBIASED)), 'montage');

    % Cleanup
    system('del input.bin');
    system('del output.bin');

end

function output = dwt2_exe(input, res)
    wbin_32('input.bin',input);
    system(sprintf('dwt %i input.bin output.bin', res), '-echo');
    output = rbin_32('output.bin', size(input));
end

function data = rbin_32(filename, n)
    fid = fopen(filename, 'r');
    data = fread(fid, n, 'int32');
    fclose(fid);
end

function wbin_32(filename, data)
    fid = fopen(filename, 'w');
    fwrite(fid, data, 'int32');
    fclose(fid);
end