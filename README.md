# OMD NewDamage

Новая модель повреждений для DayZ: органы, артерии и тупые травмы вместо «общего HP».

![Version](https://img.shields.io/badge/version-0.1.0-blue)
![Status](https://img.shields.io/badge/status-in%20development-orange)
![Enforce Script](https://img.shields.io/badge/language-Enforce%20Script-blue)

> **TL;DR (EN):** A reworked damage model for DayZ. Instead of a single global health bar, characters suffer damage to **organs** (brain, face, heart, lungs, liver, spleen, guts), with organ status HUD icons, reworked fall, melee and creature damage, heartbeat sound and screen effects. Server-configurable via JSON. Client mod + closed-source server part; without the server part the game falls back to vanilla behaviour. Written in Enforce Script.

## Возможности

- **Органы вместо общего HP:** мозг, лицо, сердце, лёгкие, печень, селезёнка, кишечник.
- **HUD:** иконки состояния органов с уровнями тяжести.
- **Переработанный урон** от падений, ближнего боя и существ.
- **Эффекты:** звук сердцебиения, эффекты экрана (размытие, свечение) в зависимости от состояния.
- Интеграция с болью Inedia и медициной/навыками Terje через `OMD_Bridge`.

## В тестировании

Эти механики реализованы, но ещё не подтверждены тестами:

- огнестрельные ранения;
- бронежилет и тупая травма;
- урон при аварии транспорта;
- выпрыгивание из транспорта на ходу;
- действие «зажать рану» (на себе и на другом игроке, включая артерии);
- сохранение состояния после переподключения.

## Требования

- DayZ (клиент и сервер)
- [OMD Bridge](https://github.com/Bambuchasensei/OMD_Bridge)
- Inedia Infected AI (`InediaInfectedAI`)
- Terje: `TerjeCore`, `TerjeMedicine`, `TerjeSkills`

## Архитектура

Мод состоит из двух частей:

- **клиентская часть** (этот репозиторий) — подключается через `-mod=` на клиенте и сервере;
- **серверная часть** `OMD_NewDamage_Server` — подключается на сервере через `-serverMod=`. Исходники серверной части закрыты.

Без `OMD_NewDamage_Server` мод откатывается к ванильному поведению урона.

## Установка

1. Подпишитесь на зависимости и на этот мод.
2. Добавьте `OMD_NewDamage` в `-mod=` клиента и сервера после Inedia, Terje и `OMD_Bridge`.
3. Добавьте `OMD_NewDamage_Server` в `-serverMod=` сервера.

## Конфигурация

При первом запуске в папке профиля сервера (`omd/`) создаются JSON-файлы:

| Файл | Назначение |
|---|---|
| `OMD_NewDamage.json` | основные параметры урона, дебаффов и лечения |
| `OMD_NewDamage_BluntTrauma.json` | тупые травмы |
| `OMD_NewDamage_Weapons.json` | параметры оружия |
| `OMD_NewDamage_Creatures.json` | урон от существ |

## Структура проекта

```
OMD_NewDamage/
├── config.cpp          # CfgPatches, зоны урона персонажа, звуки
├── mod.cpp
├── stringtable.csv
├── Sounds/             # сердцебиение
├── gui/                # иконки органов и layout HUD
└── Scripts/
    ├── 3_Game/         # JSON-конфиги, типы, эффекты экрана (PPE)
    ├── 4_World/        # органы, кровотечения, падения, транспорт, действия
    └── 5_Mission/      # HUD
```

## Скриншоты

<!-- TODO: добавить скриншоты (docs/images/...) -->
_Скоро._

## Steam Workshop

<!-- TODO: ссылка на мастерскую Steam -->
_Мод пока не опубликован в мастерской._

## Авторы

- **Comrad-Sensei** ([Bambuchasensei](https://github.com/Bambuchasensei))
- **mmetrjuk**

Сообщество и сервер **One More DayZ (OMD)**.

Thanks to Inedia and Terje.

## Лицензия

All rights reserved. Код и ассеты опубликованы для ознакомления (портфолио). Использование, копирование и перезалив — только с разрешения авторов.