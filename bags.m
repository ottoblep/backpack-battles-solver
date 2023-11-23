function types = bags()
    types.fanny_pack = bag();
    types.fanny_pack.blocks = [0 0; 1 0;];

    types.leather_bag = bag();
    types.leather_bag.blocks = [0 0; 1 0; 0 1; 1 1];

    types.potion_belt = bag();
    types.potion_belt.blocks = [0 0; 0 1; 0 2; 0 3; 0 4];

    types.protective_purse = bag();
    types.protective_purse.blocks = [0 0;];

    types.ranger_bag = bag();
    types.ranger_bag.blocks = [0 0; 0 1; 1 0; 1 1; 2 0; 2 1];

    types.stamina_sack = bag();
    types.stamina_sack.blocks = [0 0; 0 1; 0 2;];

    types.storage_coffin = bag();
    types.storage_coffin.blocks = [0 0; 0 1; 1 0; 1 1; 2 0; 2 1; 3 0; 3 1];
end