# OMD NewDamage

Новая модель повреждений для DayZ: органы, артерии, тупые травмы и смерть от потери крови и шока вместо «общего HP».

![DayZ](https://img.shields.io/badge/DayZ-1.2x-red)
![Enforce Script](https://img.shields.io/badge/language-Enforce%20Script-blue)
![Status](https://img.shields.io/badge/status-in%20development-orange)

> **TL;DR (EN):** A reworked damage model for DayZ. Instead of dying from a global health bar, characters suffer damage to **organs** (brain, heart, lungs, liver, spleen, guts, face) and **arteries**, with blunt trauma, reworked fall/vehicle/melee damage and bleeding. Includes a "press the wound" first-aid action (self and on others), organ status HUD icons, heartbeat sound and screen effects. Server-configurable via JSON. Written in Enforce Script.

## Возможности

- **Органы вместо общего HP:** мозг, лицо, сердце, лёгкие, печень, селезёнка, кишечник. Состояние каждого органа показывается иконкой на HUD с уровнями тяжести.
- **Артериальные кровотечения и «зажать рану»:** новое действие на себе и на другом игроке — зажать рану на голове, торсе, руках, кистях, ногах, стопах, а также сонную, бедренную и подключичную артерии.
- **Тупые травмы** с отдельными настройками.
- **Переработанный урон:** падения, транспорт (машины, мотоциклы, лодки, выпрыгивание на ходу), ближний бой; отдельные настройки для оружия и существ.
- **Атмосфера:** звук сердцебиения, эффекты экрана (размытие, свечение) в зависимости от состояния, влияние ранений на прицеливание.
- Интеграция с болью Inedia и медициной/навыками Terje через `OMD_Bridge`.

## Требования

- DayZ (клиент и сервер)
- [OMD Bridge](https://github.com/Bambuchasensei/OMD_Bridge)
- Inedia Infected AI — `InediaInfectedAI`
- Terje: `TerjeCore`, `TerjeMedicine`, `TerjeSkills`

## Установка

1. Подпишитесь на зависимости и на этот мод.
2. Добавьте мод в `-mod=` клиента и сервера после Inedia, Terje и `OMD_Bridge`.
3. Серверная логика вынесена в отдельный приватный репозиторий (`OMD_NewDamage_Server`) и ставится только на сервер.

## Конфигурация

При первом запуске в `%profile%/omd/` создаются JSON-файлы:

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

## Автор

**Bambuchasensei** — разработка и дизайн механик для сообщества и сервера **One More DayZ (OMD)**.

## Лицензия

All rights reserved. Код и ассеты опубликованы для ознакомления (портфолио). Использование, копирование и перезалив — только с разрешения автора.