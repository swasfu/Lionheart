# Lionheart
Game engine for personal use

## Entity-component registry
Create new components by declaring a component class extending the ComponentBase class. Create a function for building them from an XML node by specialising the BuildComponent function template in XMLParser.h, and mapping the desired XML tag name to this function using XMLParser::RegisterComponent().

# World generation
Randomly generate hex-gridded planets with plenty of customisation <br />

![Revolving](https://github.com/swasfu/Lionheart/assets/142467272/55615b18-131c-4bc6-a195-d7ca0e406ef1)
