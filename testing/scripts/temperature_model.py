temperature = 0

sunFactor = 10
sunExp = 1

spaceTemp = -270
spaceLoss = 0.007

night = 8
twilight = 2
lowLight = 4
midLight = 4
highLight = 6

nightStrength = 0
twilightStrength = 0.1
lowLightStrength = 0.3
midLightStrength = 0.7
highLightStrength = 0.9

for day in range(1000):
    averageTemp = temperature
    for i in range(24):
        temperature = ((1 - spaceLoss) * temperature + spaceLoss * spaceTemp)
    
        if i < night:
            temperature += pow(nightStrength, sunExp) * sunFactor
        elif i < night + twilight // 2:
            temperature += pow(twilightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight) // 2:
            temperature += pow(lowLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight + midLight) // 2:
            temperature += pow(midLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight + midLight) // 2 + highLight:
            temperature += pow(highLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight) // 2 + midLight + highLight:
            temperature += pow(midLightStrength, sunExp) * sunFactor
        elif i < night + (twilight) // 2 + lowLight + midLight + highLight:
            temperature += pow(lowLightStrength, sunExp) * sunFactor
        elif i < night + twilight + lowLight + midLight + highLight:
            temperature += pow(twilightStrength, sunExp) * sunFactor
    averageTemp += temperature
    averageTemp /= 2
print(averageTemp)

night = 14
twilight = 2
lowLight = 2
midLight = 2
highLight = 4

for day in range(1000):
    averageTemp = temperature
    for i in range(24):
        temperature = ((1 - spaceLoss) * temperature + spaceLoss * spaceTemp)
    
        if i < night:
            temperature += pow(nightStrength, sunExp) * sunFactor
        elif i < night + twilight // 2:
            temperature += pow(twilightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight) // 2:
            temperature += pow(lowLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight + midLight) // 2:
            temperature += pow(midLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight + midLight) // 2 + highLight:
            temperature += pow(highLightStrength, sunExp) * sunFactor
        elif i < night + (twilight + lowLight) // 2 + midLight + highLight:
            temperature += pow(midLightStrength, sunExp) * sunFactor
        elif i < night + (twilight) // 2 + lowLight + midLight + highLight:
            temperature += pow(lowLightStrength, sunExp) * sunFactor
        elif i < night + twilight + lowLight + midLight + highLight:
            temperature += pow(twilightStrength, sunExp) * sunFactor
    averageTemp += temperature
    averageTemp /= 2
print(averageTemp)
