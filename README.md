## How to build

Open Visual Studio.

Go to File > Open > Folder... en select this map

Wait until CMake is done configuring

Select card_engine.exe as start-item in the toolbar

Press F5 (Run)

All Dependencies (SFML 3) are included in the /external en will be compiled automaticly

## How to play

###### Character Selection
First you need to pick a class and character, The characters are specifically made to be 1 class but you can mix them with each other (If you don't know what to pick: just pick for example the 4th class and 4th character. That way you get a combo that was made for each other)

All classes have a different deck, mana, hp, shield and corruption. Each character gives a unique bonus and gives different effects that happen when you transform and when you play a card while transformed.

|    Class Name    | HP  | Shield | Mana | Corruption |
| :--------------: | :-: | :----: | :--: | :--------: |
|   **Warrior**    | 80  |   30   |  2   |     1      |
|    **Cleric**    | 60  |   10   |  3   |     2      |
|     **Mage**     | 30  |   10   |  4   |     3      |
|    **Ranger**    | 50  |   10   |  3   |     2      |
| **Necromancer**  | 60  |   8    |  3   |     2      |
|  **Alchemist**   | 60  |   10   |  3   |     2      |
| **Technomancer** | 50  |   30   |  5   |     3      |
|  **War Angel**   | 55  |   5    |  3   |     10     |

|  Character Name  |                      Bonus                      |                                                 On Transform                                                  |                                                 Card Play                                                 |
| :--------------: | :---------------------------------------------: | :-----------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------: |
|   **Hiroshi**    |                  10 Max shield                  |                                                Gain 10 shield                                                 | If your shield > 50% max shield: Deal damage to yourself and the enemy, If you have less: you gain shield |
|    **Phlox**     |               Phlox's Bloom card                |                                           Heal 5 and gain 5 Blessed                                           |                                  Heal 1 and give the enemy 1 Damage Down                                  |
|    **Vortex**    |                   1 Max mana                    |                                               Gain 2 True Void                                                |                    Deal 1 damage, If you are at max corruption: Gain 1 Corrupted Void                     |
| **Mathews Lift** |             - 5 Max hp, Blood Lust              | If you are at max hp: deal damage + apply bleed + Gain Damage Up, If you are not at max: Gain Locked and heal |                              Same effect as On Transform but with less value                              |
|     **Djin**     | 5 Max hp, zombie / skeleton army, Soul Fragment |                    Gain 1 zombie/ skeleton army, apply 1 Death Mark + Defence Down, Heal 3                    |                                   If you are at max corruption: Heal 8                                    |
|    **Kobalt**    |                   1 Max mana                    |                                               Gain 2 True Void                                                |                                      Create 2 random metals in hand                                       |
|    **1X NAO**    |          Apply Weaker Creator to enemy          |                         After you play the next card you create Machine Power in hand                         |                                       Gain a random Machine Upgrade                                       |
|    **Roshka**    |       5 Max corruption, Forbidden Droplet       |                            Gain 1 Flight + 2 Holy Spirit + 3 Blessed  + 10 shield                             |                                         Heal 5 and gain 5 Blessed                                         |

###### Battle
After picking your character you will be put in the first fight. In this fight you have to play cards to defeat the enemy while taking the least amount of damage. To achieve this you have to play cards.

You can see your hand at the bottom of the screen, At the bottom right side are your stats. From top to bottom they are: Corruption, HP (and shield) and Mana.
Above your healthbar there can also be a status bar, This shows all currently gained statuses you have. You can scroll inside the status bas if you have alot of statuses.
If you dont know what a certain card or status does you can hover over it to see a tooltip.

Each card has a: cost, value, corrupted value and effect. You can click a card to play it (If you have enough mana). After the card has been resolved you gain +1 corruption. 
While you are at the max corruption the corrupted value gets add to the normal value. (Value just means the amount of damage for example).  

At the cost of your max mana you can transform for 2 turns. 
When you transform; a character effects activates and every card you play activates another effect.
Some cards have special interactions if you are transformed. This is highlighted like this: "Deal damage 3 times (+ Heal each hit)" This means that if you are not transformed you don't heal. If it doesn't have the + it means it's replacing the effect.

If you want to end your turn you can click the button. After you've ended your turn the enemy attacks, You can see what kind of attack the enemy is doing. When its your turn again you regain all mana and draw 4 new cards.

###### Post Battle
After you've won from the fight you get to pick 1 out of 3 cards to add to your deck. You can also skip this by just clicking continue. After 3 wins in a row You go to either "The Orb" or "The Forge"

**The Orb:** Click a card you wish to upgrade, At the bottom of the screen you see 3 "Imprints" you can click on of those to add to that card (They activate once you play the card). The imprint only goes on 1 copy not all of the same card. You can do this twice. 

**The Forge:** You can select 2 cards, Transform them both into 1 powerful fusion card. If you picked 2 fusion cards to transform they become a even stronger fusion card.

After every 3 fights the difficulty goes up. After 12 fights you have won!

###### Small Notes
For Alchemist here is a list of every effect "Reaction" can have:

|  Metal  |  Metal  |      Name      |                    Effect                    |
| :-----: | :-----: | :------------: | :------------------------------------------: |
| Copper  |  Iron   |     Bronze     |         Gain 8 shield + 3 Defence Up         |
| Copper  |   Tin   |     Brass      |         3 Damage Up + Deal 6 damage          |
| Copper  |  Gold   |  Aurichalceum  |            Deal 7 damage + Heal 7            |
| Copper  | Mercury |   Verdigris    |                Apply 9 Bleed                 |
| Copper  |  Lead   |   White Metal  |             Stun + Apply 2 Bleed             |
| Copper  | Silver  |   Conducting   |         Gain 1 mana + Deal 8 damage          |
|  Gold   | Silver  |    Electrum    |                   Heal 30                    |
|  Gold   |  Lead   |     Midas      |       Gain 5 Damage Up + Deal 8 damage       |
|  Gold   |  Iron   | Electrum Steel | Deal 5 damage 2 times + Apply 3 Defence Down |
|  Gold   | Mercury |   Shimmering   |          Gain 3 True Void + Heal 5           |
|  Gold   |   Tin   |     Bright     |       Gain 4 Damage Up + Deal 5 damage       |
| Mercury | Silver  |  Quicksilver   |            Deal 4 damage 3 times             |
| Mercury |  Iron   |    Volatile    |        Deal 15 damage + Take 5 damage        |
| Mercury |  Lead   |    Amalgam     |            Deal 4 damage + Heal 3            |
| Mercury |   Tin   |     Fluid      |    Deal 6 damage 2 times + Take 2 damage     |
|  Iron   |   Tin   |     Steel      |        Gain 10 shield + 2 Defence Up         |
|  Iron   |  Lead   |     Solder     |                  Apply Stun                  |
|  Iron   | Silver  |   Shiny Hard   |         Gain 7 shield + 2 Defence Up         |
|   Tin   |  Lead   |     Pewter     |        Deal 12 damage + Take 3 damage        |
|   Tin   | Silver  |    Brittle     |     Deal 9 damage + Apply 2 Damage Down      |
