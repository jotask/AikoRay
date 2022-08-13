/*******************************************************************************************
*
*   raygui v3.2 - A simple and easy-to-use immediate-mode gui library
*
*   DESCRIPTION:
*
*   raygui is a tools-dev-focused immediate-mode-gui library based on raylib but also
*   available as a standalone library, as long as input and drawing functions are provided.
*
*   Controls provided:
*
*   # Container/separators Controls
*       - WindowBox     --> StatusBar, Panel
*       - GroupBox      --> Line
*       - Line
*       - Panel         --> StatusBar
*       - ScrollPanel   --> StatusBar
*
*   # Basic Controls
*       - Label
*       - Button
*       - LabelButton   --> Label
*       - Toggle
*       - ToggleGroup   --> Toggle
*       - CheckBox
*       - ComboBox
*       - DropdownBox
*       - TextBox
*       - TextBoxMulti
*       - ValueBox      --> TextBox
*       - Spinner       --> Button, ValueBox
*       - Slider
*       - SliderBar     --> Slider
*       - ProgressBar
*       - StatusBar
*       - DummyRec
*       - Grid
*
*   # Advance Controls
*       - ListView
*       - ColorPicker   --> ColorPanel, ColorBarHue
*       - MessageBox    --> Window, Label, Button
*       - TextInputBox  --> Window, Label, TextBox, Button
*
*   It also provides a set of functions for styling the controls based on its properties (size, color).
*
*
*   RAYGUI STYLE (guiStyle):
*
*   raygui uses a global data array for all gui style properties (allocated on data segment by default),
*   when a new style is loaded, it is loaded over the global style... but a default gui style could always be
*   recovered with GuiLoadStyleDefault() function, that overwrites the current style to the default one
*
*   The global style array size is fixed and depends on the number of controls and properties:
*
*       static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)];
*
*   guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
*
*   Note that the first set of BASE properties (by default guiStyle[0..15]) belong to the generic style
*   used for all controls, when any of those base values is set, it is automatically populated to all
*   controls, so, specific control values overwriting generic style should be set after base values.
*
*   After the first BASE set we have the EXTENDED properties (by default guiStyle[16..23]), those
*   properties are actually common to all controls and can not be overwritten individually (like BASE ones)
*   Some of those properties are: TEXT_SIZE, TEXT_SPACING, LINE_COLOR, BACKGROUND_COLOR
*
*   Custom control properties can be defined using the EXTENDED properties for each independent control.
*
*   TOOL: rGuiStyler is a visual tool to customize raygui style.
*
*
*   RAYGUI ICONS (guiIcons):
*
*   raygui could use a global array containing icons data (allocated on data segment by default),
*   a custom icons set could be loaded over this array using GuiLoadIcons(), but loaded icons set
*   must be same RAYGUI_ICON_SIZE and no more than RAYGUI_ICON_MAX_ICONS will be loaded
*
*   Every icon is codified in binary form, using 1 bit per pixel, so, every 16x16 icon
*   requires 8 integers (16*16/32) to be stored in memory.
*
*   When the icon is draw, actually one quad per pixel is drawn if the bit for that pixel is set.
*
*   The global icons array size is fixed and depends on the number of icons and size:
*
*       static unsigned int guiIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS];
*
*   guiIcons size is by default: 256*(16*16/32) = 2048*4 = 8192 bytes = 8 KB
*
*   TOOL: rGuiIcons is a visual tool to customize raygui icons.
*
*
*   CONFIGURATION:
*
*   #define RAYGUI_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
*
*   #define RAYGUI_STANDALONE
*       Avoid raylib.h header inclusion in this file. Data types defined on raylib are defined
*       internally in the library and input management and drawing functions must be provided by
*       the user (check library implementation for further details).
*
*   #define RAYGUI_NO_ICONS
*       Avoid including embedded ricons data (256 icons, 16x16 pixels, 1-bit per pixel, 2KB)
*
*   #define RAYGUI_CUSTOM_ICONS
*       Includes custom ricons.h header defining a set of custom icons,
*       this file can be generated using rGuiIcons tool
*
*
*   VERSIONS HISTORY:
*       3.2 (22-May-2022) RENAMED: Some enum values, for unification, avoiding prefixes
*                         REMOVED: GuiScrollBar(), only internal
*                         REDESIGNED: GuiPanel() to support text parameter
*                         REDESIGNED: GuiScrollPanel() to support text parameter
*                         REDESIGNED: GuiColorPicker() to support text parameter
*                         REDESIGNED: GuiColorPanel() to support text parameter
*                         REDESIGNED: GuiColorBarAlpha() to support text parameter
*                         REDESIGNED: GuiColorBarHue() to support text parameter
*                         REDESIGNED: GuiTextInputBox() to support password
*       3.1 (12-Jan-2022) REVIEWED: Default style for consistency (aligned with rGuiLayout v2.5 tool)
*                         REVIEWED: GuiLoadStyle() to support compressed font atlas image data and unload previous textures
*                         REVIEWED: External icons usage logic
*                         REVIEWED: GuiLine() for centered alignment when including text
*                         RENAMED: Multiple controls properties definitions to prepend RAYGUI_
*                         RENAMED: RICON_ references to RAYGUI_ICON_ for library consistency
*                         Projects updated and multiple tweaks
*       3.0 (04-Nov-2021) Integrated ricons data to avoid external file
*                         REDESIGNED: GuiTextBoxMulti()
*                         REMOVED: GuiImageButton*()
*                         Multiple minor tweaks and bugs corrected
*       2.9 (17-Mar-2021) REMOVED: Tooltip API
*       2.8 (03-May-2020) Centralized raylib::Rectangles drawing to GuiDrawraylib::Rectangle()
*       2.7 (20-Feb-2020) ADDED: Possible tooltips API
*       2.6 (09-Sep-2019) ADDED: GuiTextInputBox()
*                         REDESIGNED: GuiListView*(), GuiDropdownBox(), GuiSlider*(), GuiProgressBar(), GuiMessageBox()
*                         REVIEWED: GuiTextBox(), GuiSpinner(), GuiValueBox(), GuiLoadStyle()
*                         Replaced property INNER_PADDING by TEXT_PADDING, renamed some properties
*                         ADDED: 8 new custom styles ready to use
*                         Multiple minor tweaks and bugs corrected
*       2.5 (28-May-2019) Implemented extended GuiTextBox(), GuiValueBox(), GuiSpinner()
*       2.3 (29-Apr-2019) ADDED: rIcons auxiliar library and support for it, multiple controls reviewed
*                         Refactor all controls drawing mechanism to use control state
*       2.2 (05-Feb-2019) ADDED: GuiScrollBar(), GuiScrollPanel(), reviewed GuiListView(), removed Gui*Ex() controls
*       2.1 (26-Dec-2018) REDESIGNED: GuiCheckBox(), GuiComboBox(), GuiDropdownBox(), GuiToggleGroup() > Use combined text string
*                         REDESIGNED: Style system (breaking change)
*       2.0 (08-Nov-2018) ADDED: Support controls guiLock and custom fonts
*                         REVIEWED: GuiComboBox(), GuiListView()...
*       1.9 (09-Oct-2018) REVIEWED: GuiGrid(), GuiTextBox(), GuiTextBoxMulti(), GuiValueBox()...
*       1.8 (01-May-2018) Lot of rework and redesign to align with rGuiStyler and rGuiLayout
*       1.5 (21-Jun-2017) Working in an improved styles system
*       1.4 (15-Jun-2017) Rewritten all GUI functions (removed useless ones)
*       1.3 (12-Jun-2017) Complete redesign of style system
*       1.1 (01-Jun-2017) Complete review of the library
*       1.0 (07-Jun-2016) Converted to header-only by Ramon Santamaria.
*       0.9 (07-Mar-2016) Reviewed and tested by Albert Martos, Ian Eito, Sergio Martinez and Ramon Santamaria.
*       0.8 (27-Aug-2015) Initial release. Implemented by Kevin Gato, Daniel Nicolás and Ramon Santamaria.
*
*
*   CONTRIBUTORS:
*
*       Ramon Santamaria:   Supervision, review, redesign, update and maintenance
*       Vlad Adrian:        Complete rewrite of GuiTextBox() to support extended features (2019)
*       Sergio Martinez:    Review, testing (2015) and redesign of multiple controls (2018)
*       Adria Arranz:       Testing and Implementation of additional controls (2018)
*       Jordi Jorba:        Testing and Implementation of additional controls (2018)
*       Albert Martos:      Review and testing of the library (2015)
*       Ian Eito:           Review and testing of the library (2015)
*       Kevin Gato:         Initial implementation of basic components (2014)
*       Daniel Nicolas:     Initial implementation of basic components (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#pragma once

#include "utils/raylib_include.hpp"
#include "core/singleton.hpp"

namespace aiko
{

    namespace raylib_helper
    {

        class RayGui : aiko::Singleton<RayGui>
        {

        private:
            RayGui();

        public:

            static constexpr const int RAYGUI_MAX_VERSION = 3;
            static constexpr const int RAYGUI_MIN_VERSION = 2;

            // Style property
            typedef struct GuiStyleProp {
                unsigned short controlId;
                unsigned short propertyId;
                unsigned int propertyValue;
            } GuiStyleProp;

            typedef enum { BORDER = 0, BASE, TEXT, OTHER } GuiPropertyElement;

            // Gui control state
            typedef enum {
                STATE_NORMAL = 0,
                STATE_FOCUSED,
                STATE_PRESSED,
                STATE_DISABLED,
            } GuiState;

            // Gui control text alignment
            typedef enum {
                TEXT_ALIGN_LEFT = 0,
                TEXT_ALIGN_CENTER,
                TEXT_ALIGN_RIGHT,
            } GuiTextAlignment;

            // Gui controls
            typedef enum {
                // Default -> populates to all controls when set
                DEFAULT = 0,
                // Basic controls
                LABEL,          // Used also for: LABELBUTTON
                BUTTON,
                TOGGLE,         // Used also for: TOGGLEGROUP
                SLIDER,         // Used also for: SLIDERBAR
                PROGRESSBAR,
                CHECKBOX,
                COMBOBOX,
                DROPDOWNBOX,
                TEXTBOX,        // Used also for: TEXTBOXMULTI
                VALUEBOX,
                SPINNER,        // Uses: BUTTON, VALUEBOX
                LISTVIEW,
                COLORPICKER,
                SCROLLBAR,
                STATUSBAR
            } GuiControl;

            // Gui base properties for every control
            // NOTE: RAYGUI_MAX_PROPS_BASE properties (by default 16 properties)
            typedef enum {
                BORDER_COLOR_NORMAL = 0,
                BASE_COLOR_NORMAL,
                TEXT_COLOR_NORMAL,
                BORDER_COLOR_FOCUSED,
                BASE_COLOR_FOCUSED,
                TEXT_COLOR_FOCUSED,
                BORDER_COLOR_PRESSED,
                BASE_COLOR_PRESSED,
                TEXT_COLOR_PRESSED,
                BORDER_COLOR_DISABLED,
                BASE_COLOR_DISABLED,
                TEXT_COLOR_DISABLED,
                BORDER_WIDTH,
                TEXT_PADDING,
                TEXT_ALIGNMENT,
                RESERVED
            } GuiControlProperty;

            // Gui extended properties depend on control
            // NOTE: RAYGUI_MAX_PROPS_EXTENDED properties (by default 8 properties)
            //----------------------------------------------------------------------------------

            // DEFAULT extended properties
            // NOTE: Those properties are common to all controls or global
            typedef enum {
                TEXT_SIZE = 16,             // Text size (glyphs max height)
                TEXT_SPACING,               // Text spacing between glyphs
                LINE_COLOR,                 // Line control color
                BACKGROUND_COLOR,           // Background color
            } GuiDefaultProperty;

            // Label
            //typedef enum { } GuiLabelProperty;

            // Button/Spinner
            //typedef enum { } GuiButtonProperty;

            // Toggle/ToggleGroup
            typedef enum {
                GROUP_PADDING = 16,         // ToggleGroup separation between toggles
            } GuiToggleProperty;

            // Slider/SliderBar
            typedef enum {
                SLIDER_WIDTH = 16,          // Slider size of internal bar
                SLIDER_PADDING              // Slider/SliderBar internal bar padding
            } GuiSliderProperty;

            // ProgressBar
            typedef enum {
                PROGRESS_PADDING = 16,      // ProgressBar internal padding
            } GuiProgressBarProperty;

            // ScrollBar
            typedef enum {
                ARROWS_SIZE = 16,
                ARROWS_VISIBLE,
                SCROLL_SLIDER_PADDING,      // (SLIDERBAR, SLIDER_PADDING)
                SCROLL_SLIDER_SIZE,
                SCROLL_PADDING,
                SCROLL_SPEED,
            } GuiScrollBarProperty;

            // CheckBox
            typedef enum {
                CHECK_PADDING = 16          // CheckBox internal check padding
            } GuiCheckBoxProperty;

            // ComboBox
            typedef enum {
                COMBO_BUTTON_WIDTH = 16,    // ComboBox right button width
                COMBO_BUTTON_SPACING        // ComboBox button separation
            } GuiComboBoxProperty;

            // DropdownBox
            typedef enum {
                ARROW_PADDING = 16,         // DropdownBox arrow separation from border and items
                DROPDOWN_ITEMS_SPACING      // DropdownBox items separation
            } GuiDropdownBoxProperty;

            // TextBox/TextBoxMulti/ValueBox/Spinner
            typedef enum {
                TEXT_INNER_PADDING = 16,    // TextBox/TextBoxMulti/ValueBox/Spinner inner text padding
                TEXT_LINES_SPACING,         // TextBoxMulti lines separation
            } GuiTextBoxProperty;

            // Spinner
            typedef enum {
                SPIN_BUTTON_WIDTH = 16,     // Spinner left/right buttons width
                SPIN_BUTTON_SPACING,        // Spinner buttons separation
            } GuiSpinnerProperty;

            // ListView
            typedef enum {
                LIST_ITEMS_HEIGHT = 16,     // ListView items height
                LIST_ITEMS_SPACING,         // ListView items separation
                SCROLLBAR_WIDTH,            // ListView scrollbar size (usually width)
                SCROLLBAR_SIDE,             // ListView scrollbar side (0-left, 1-right)
            } GuiListViewProperty;

            // ColorPicker
            typedef enum {
                COLOR_SELECTOR_SIZE = 16,
                HUEBAR_WIDTH,               // ColorPicker right hue bar width
                HUEBAR_PADDING,             // ColorPicker right hue bar separation from panel
                HUEBAR_SELECTOR_HEIGHT,     // ColorPicker right hue bar selector height
                HUEBAR_SELECTOR_OVERFLOW    // ColorPicker right hue bar selector overflow
            } GuiColorPickerProperty;

            static constexpr const int SCROLLBAR_LEFT_SIDE = 0;
            static constexpr const int SCROLLBAR_RIGHT_SIDE = 1;
            static constexpr const int RAYGUI_MAX_CONTROLS = 16;
            static constexpr const int RAYGUI_MAX_PROPS_BASE = 16;
            static constexpr const int RAYGUI_MAX_PROPS_EXTENDED = 8;


            static constexpr const int RAYGUI_ICON_SIZE = 16;
            static constexpr const int RAYGUI_ICON_MAX_ICONS = 256;
            static constexpr const int RAYGUI_ICON_MAX_NAME_LENGTH = 32;

            //----------------------------------------------------------------------------------
            // Global Variables Definition
            //----------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------
            // Global Variables Definition
            //----------------------------------------------------------------------------------
            GuiState guiState;    // Gui global state, if !STATE_NORMAL, forces defined state
            raylib::Font guiFont;        // Gui current font (WARNING: highly coupled to raylib)
            bool guiLocked;              // Gui lock state (no inputs processed)
            float guiAlpha;               // Gui element transpacency on drawing
            unsigned int guiIconScale;       // Gui icon default scale (if icons enabled)

            //----------------------------------------------------------------------------------
            // Style data array for all gui style properties (allocated on data segment by default)
            //
            // NOTE 1: First set of BASE properties are generic to all controls but could be individually
            // overwritten per control, first set of EXTENDED properties are generic to all controls and
            // can not be overwritten individually but custom EXTENDED properties can be used by control
            //
            // NOTE 2: A new style set could be loaded over this array using GuiLoadStyle(),
            // but default gui style could always be recovered with GuiLoadStyleDefault()
            //
            // guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
            //----------------------------------------------------------------------------------
            unsigned int guiStyle[RAYGUI_MAX_CONTROLS * (RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)];

            bool guiStyleLoaded;         // Style loaded flag for lazy style initialization

            //----------------------------------------------------------------------------------
            // Module specific Functions Declaration
            //----------------------------------------------------------------------------------
            int GetTextWidth(const char* text);                      // Gui get text width using default font
            raylib::Rectangle GetTextBounds(int control, raylib::Rectangle bounds);  // Get text bounds considering control bounds
            const char* GetTextIcon(const char* text, int* iconId);  // Get text icon if provided and move text cursor

            void GuiDrawText(const char* text, raylib::Rectangle bounds, int alignment, raylib::Color tint);         // Gui draw text using default font
            void GuiDrawRectangle(raylib::Rectangle rec, int borderWidth, raylib::Color borderColor, raylib::Color color);   // Gui draw rectangle using default raygui style

            const char** GuiTextSplit(const char* text, int* count, int* textRow);       // Split controls text into multiple strings
            raylib::Vector3 ConvertHSVtoRGB(raylib::Vector3 hsv);                    // Convert color data from HSV to RGB
            raylib::Vector3 ConvertRGBtoHSV(raylib::Vector3 rgb);                    // Convert color data from RGB to HSV
            float GuiSliderPro(raylib::Rectangle bounds, const char* textLeft, const char* textRight, float value, float minValue, float maxValue, int sliderWidth);

            int GuiScrollBar(raylib::Rectangle bounds, int value, int minValue, int maxValue);   // Scroll bar control, used by GuiScrollPanel()

            // Global gui state control functions
            void GuiEnable(void);                                         // Enable gui controls (global state)
            void GuiDisable(void);                                        // Disable gui controls (global state)
            void GuiLock(void);                                           // Lock gui controls (global state)
            void GuiUnlock(void);                                         // Unlock gui controls (global state)
            bool GuiIsLocked(void);                                       // Check if gui is locked (global state)
            void GuiFade(float alpha);                                    // Set gui controls alpha (global state), alpha goes from 0.0f to 1.0f
            void GuiSetState(int state);                                  // Set gui state (global state)
            RayGui::GuiState GuiGetState(void);                                        // Get gui state (global state)

            // Font set/get functions
            void GuiSetFont(raylib::Font font);                                   // Set gui custom font (global state)
            raylib::Font GuiGetFont(void);                                        // Get gui custom font (global state)

            // Style set/get functions
            void GuiSetStyle(int control, int property, int value);       // Set one style property
            int GuiGetStyle(int control, int property);                   // Get one style property

            // Container/separator controls, useful for controls organization
            bool GuiWindowBox(raylib::Rectangle bounds, const char* title);                                       // Window Box control, shows a window that can be closed
            void GuiGroupBox(raylib::Rectangle bounds, const char* text);                                         // Group Box control with text name
            void GuiLine(raylib::Rectangle bounds, const char* text);                                             // Line separator control, could contain text
            void GuiPanel(raylib::Rectangle bounds, const char* text);                                            // Panel control, useful to group controls
            raylib::Rectangle GuiScrollPanel(raylib::Rectangle bounds, const char* text, raylib::Rectangle content, raylib::Vector2* scroll); // Scroll Panel control

            // Basic controls set
            void GuiLabel(raylib::Rectangle bounds, const char* text);                                            // Label control, shows text
            bool GuiButton(raylib::Rectangle bounds, const char* text);                                           // Button control, returns true when clicked
            bool GuiLabelButton(raylib::Rectangle bounds, const char* text);                                      // Label button control, show true when clicked
            bool GuiToggle(raylib::Rectangle bounds, const char* text, bool active);                              // Toggle Button control, returns true when active
            int GuiToggleGroup(raylib::Rectangle bounds, const char* text, int active);                           // Toggle Group control, returns active toggle index
            bool GuiCheckBox(raylib::Rectangle bounds, const char* text, bool checked);                           // Check Box control, returns true when active
            int GuiComboBox(raylib::Rectangle bounds, const char* text, int active);                              // Combo Box control, returns selected item index
            bool GuiDropdownBox(raylib::Rectangle bounds, const char* text, int* active, bool editMode);          // Dropdown Box control, returns selected item
            bool GuiSpinner(raylib::Rectangle bounds, const char* text, int* value, int minValue, int maxValue, bool editMode);     // Spinner control, returns selected value
            bool GuiValueBox(raylib::Rectangle bounds, const char* text, int* value, int minValue, int maxValue, bool editMode);    // Value Box control, updates input text with numbers
            bool GuiTextBox(raylib::Rectangle bounds, char* text, int textSize, bool editMode);                   // Text Box control, updates input text
            bool GuiTextBoxMulti(raylib::Rectangle bounds, char* text, int textSize, bool editMode);              // Text Box control with multiple lines
            float GuiSlider(raylib::Rectangle bounds, const char* textLeft, const char* textRight, float value, float minValue, float maxValue);       // Slider control, returns selected value
            float GuiSliderBar(raylib::Rectangle bounds, const char* textLeft, const char* textRight, float value, float minValue, float maxValue);    // Slider Bar control, returns selected value
            float GuiProgressBar(raylib::Rectangle bounds, const char* textLeft, const char* textRight, float value, float minValue, float maxValue);  // Progress Bar control, shows current progress value
            void GuiStatusBar(raylib::Rectangle bounds, const char* text);                                        // Status Bar control, shows info text
            void GuiDummyRec(raylib::Rectangle bounds, const char* text);                                         // Dummy control for placeholders
            raylib::Vector2 GuiGrid(raylib::Rectangle bounds, const char* text, float spacing, int subdivs);              // Grid control, returns mouse cell position

            // Advance controls set
            int GuiListView(raylib::Rectangle bounds, const char* text, int* scrollIndex, int active);            // List View control, returns selected list item index
            int GuiListViewEx(raylib::Rectangle bounds, const char** text, int count, int* focus, int* scrollIndex, int active);      // List View with extended parameters
            int GuiMessageBox(raylib::Rectangle bounds, const char* title, const char* message, const char* buttons);                 // Message Box control, displays a message
            int GuiTextInputBox(raylib::Rectangle bounds, const char* title, const char* message, const char* buttons, char* text, int textMaxSize, int* secretViewActive);   // Text Input Box control, ask for text, supports secret
            raylib::Color GuiColorPicker(raylib::Rectangle bounds, const char* text, raylib::Color color);                        // Color Picker control (multiple color controls)
            raylib::Color GuiColorPanel(raylib::Rectangle bounds, const char* text, raylib::Color color);                         // Color Panel control
            float GuiColorBarAlpha(raylib::Rectangle bounds, const char* text, float alpha);                      // Color Bar Alpha control
            float GuiColorBarHue(raylib::Rectangle bounds, const char* text, float value);                        // Color Bar Hue control

            // Styles loading functions
            void GuiLoadStyle(const char* fileName);              // Load style file over global style variable (.rgs)
            void GuiLoadStyleDefault(void);                       // Load style default over global style

            // Icons functionality
            const char* GuiIconText(int iconId, const char* text); // Get text with icon id prepended (if supported)

            void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, raylib::Color color);

            unsigned int* GuiGetIcons(void);                      // Get full icons data pointer
            unsigned int* GuiGetIconData(int iconId);             // Get icon bit data
            void GuiSetIconData(int iconId, unsigned int* data);  // Set icon bit data
            void GuiSetIconScale(unsigned int scale);             // Set icon scale (1 by default)

            void GuiSetIconPixel(int iconId, int x, int y);       // Set icon pixel value
            void GuiClearIconPixel(int iconId, int x, int y);     // Clear icon pixel value
            bool GuiCheckIconPixel(int iconId, int x, int y);     // Check icon pixel value

        };

    }

}
