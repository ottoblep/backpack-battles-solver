% Defines the properties a bag that can contain items 
classdef bag
    properties 
        position int32
        % 1x2 integer array = coordinates 
        rotation int32
        % 1=90degrees 2=180degrees etc
        blocks int32
        % 2xN array of relative coordinates relative to origin
        bag_type
        % Decides which other items can be modified by this bag
    end
end