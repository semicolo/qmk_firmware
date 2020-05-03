#pragma once

#define MACROMAXLENGTH 20

static const char macroNames[][16] = {
"   MACRO 1      ",
"   MACRO 2      ",
"   MACRO 3      ",
"   MACRO 4      ",
"   MACRO 5      ",
"   MACRO 6      ",
"   MACRO 7      ",
"   MACRO 8      ",
"   MACRO 9      ",
"   MACRO 10     ",
"   MACRO 11     ",
"   MACRO 12     ",
"   MACRO 13     ",
"   MACRO 14     ",
"   MACRO 15     ",
"   MACRO 16     "
};

static const char macros[][MACROMAXLENGTH] = {
"Macro string 1",
"Macro string 2",
"Macro string 3",
"Macro string 4",
"Macro string 5",
"Macro string 6",
"Macro string 7",
"Macro string 8",
"Macro string 9",
"Macro string 10",
"Macro string 11",
"Macro string 12",
"Macro string 13",
"Macro string 14",
"Macro string 15",
"Macro string 16"
};

#define MACROSNUMBER (sizeof(macros)/MACROMAXLENGTH)
