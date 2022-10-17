-Запускать с карты Content\Maps\MainMenuMap
	стоит картой по умолчанию в редакторе

-В блупринтах логики нет, все написано на с++
  Почти все с++ классы абстрактные, и имею потомков в блупринтах для настройки параметров

-Основные классы и пути до них:
	Source\JoyTest\Public\GameObjects\
   	JT_GameHUD.h
	JT_GameInstance.h
	JT_GameModeBase.h
	JT_PlayerController.h
	JT_DamageInfoActor.h
	
-Интерактивные объекты
	Source\JoyTest\Public\InteractiveObjects\
-Инвентарь
	Source\JoyTest\Public\Components\
	JT_InteractiveComponent.h
-Персонаж 
	Source\JoyTest\Public\Character\
	JT_CharacterBase.h
-Ловушки
	Source\JoyTest\Public\Traps
-Виджеты
	Source\JoyTest\Public\Widgets


-Назначение клавиш
 I - инвентарь
 E - взаимодействие
 1,2,3,4,5 - Слоты для быстроко доступа в инвентаре

Перемещать вещи в инвенторе можно правой клавишей или через Drag&Drop

Блупринты находятся в такихже папках в корне проекта

Возможно, на момент проверки, я чтото неуспел засетапить или пофиксить, заранее извиняюсь...