function types = block_types()
    % Define Blocks
    types.simple_block = item();
    types.simple_block.blocks = [0 0];
    types.simple_block.connection_blocks = [];

    types.double_block = item();
    types.double_block.blocks = [0 0; 0 1];
    types.double_block.connection_blocks = [0 -1; 0 2];

    types.corner_block = item();
    types.corner_block.blocks = [0 0; 0 1; 1 1];
    types.corner_block.connection_blocks = [0 -1; 2 1];

    types.square_block = item();
    types.square_block.blocks = [0 0; 0 1; 1 1; 1 0];
    types.square_block.connection_blocks = [0 -1; 1 -1];
end