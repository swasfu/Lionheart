# Lionheart
Game engine for personal use

##Entity-component registry
Create new components by declaring a component class extending the ComponentBase class. Create a function for building them from an XML node by specialising the BuildComponent function template in XMLParser.h, and mapping the desired XML tag name to this function using XMLParser::RegisterComponent().
