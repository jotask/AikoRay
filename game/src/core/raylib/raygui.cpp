#include "raygui.hpp"

#ifndef RAYGUI_DISABLED

namespace aiko
{

    namespace raylib_helper
    {

#define RAYGUI_NO_ICONS

        RayGui::RayGui()
            : guiState(GuiState::STATE_NORMAL)
            , guiFont( { 0 } )
            , guiLocked( false )
            , guiAlpha( 1.0f )
            , guiIconScale( 1 )
            , guiStyle( )
            , guiStyleLoaded( false )
        {

        }

        //----------------------------------------------------------------------------------
        // Gui Setup Functions Definition
        //----------------------------------------------------------------------------------
        // Enable gui global state
        // NOTE: We check for GuiState::STATE_DISABLED to avoid messing custom global state setups
        //----------------------------------------------------------------------------------
        void RayGui::GuiEnable(void)
        {
            if (guiState == GuiState::STATE_DISABLED)
            {
                guiState = GuiState::STATE_NORMAL;
            }
        }

        // Disable gui global state
        // NOTE: We check for STATE_NORMAL to avoid messing custom global state setups
        void RayGui::GuiDisable(void)
        {
            if (guiState == GuiState::STATE_NORMAL)
            {
                guiState = GuiState::STATE_DISABLED;
            }
        }

        // Lock gui global state
        void RayGui::GuiLock(void)
        {
            guiLocked = true;
        }

        // Unlock gui global state
        void RayGui::GuiUnlock(void)
        {
            guiLocked = false;
        }

        // Check if gui is locked (global state)
        bool RayGui::GuiIsLocked(void)
        {
            return guiLocked;
        }

        // Set gui controls alpha global state
        void RayGui::GuiFade(float alpha)
        {
            if (alpha < 0.0f)
            {
                alpha = 0.0f;
            }
            else if (alpha > 1.0f)
            {
                alpha = 1.0f;
            }
            guiAlpha = alpha;
        }

        // Set gui state (global state)
        void RayGui::GuiSetState(int state)
        {
            guiState = (RayGui::GuiState)state;
        }

        // Get gui state (global state)
        RayGui::GuiState RayGui::GuiGetState(void)
        {
            return guiState;
        }

        // Set custom gui font
        // NOTE: Font loading/unloading is external to raygui
        void RayGui::GuiSetFont(raylib::Font font)
        {
            if (font.texture.id > 0)
            {
                // NOTE: If we try to setup a font but default style has not been
                // lazily loaded before, it will be overwritten, so we need to force
                // default style loading first
                if (!guiStyleLoaded)
                {
                    GuiLoadStyleDefault();
                }
                guiFont = font;
                GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, font.baseSize);
            }
        }

        // Get custom gui font
        raylib::Font RayGui::GuiGetFont(void)
        {
            return guiFont;
        }

        // Set control style property value
        void RayGui::GuiSetStyle(int control, int property, int value)
        {
            if (!guiStyleLoaded) GuiLoadStyleDefault();
            guiStyle[static_cast<int>(control)*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + static_cast<int>(property)] = value;

            // Default properties are propagated to all controls
            if ((static_cast<int>(control) == 0) && (static_cast<int>(property) < RAYGUI_MAX_PROPS_BASE))
            {
                for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) guiStyle[i*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + static_cast<int>(property)] = value;
            }
        }

        // Get control style property value
        int RayGui::GuiGetStyle(int control, int property)
        {
            if (!guiStyleLoaded)
            {
                GuiLoadStyleDefault();
            }
            return guiStyle[static_cast<int>(control) *(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + static_cast<int>(property)];
        }

        //----------------------------------------------------------------------------------
        // Gui Controls Functions Definition
        //----------------------------------------------------------------------------------

        // Window Box control
        bool RayGui::GuiWindowBox(raylib::Rectangle bounds, const char *title)
        {
            // Window title bar height (including borders)
            // NOTE: This define is also used by GuiMessageBox() and GuiTextInputBox()
            #if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
                #define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT        24
            #endif

            //GuiState state = guiState;
            bool clicked = false;

            int statusBarHeight = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;

            raylib::Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)statusBarHeight };
            if (bounds.height < statusBarHeight*2.0f) bounds.height = statusBarHeight*2.0f;

            raylib::Rectangle windowPanel = { bounds.x, bounds.y + (float)statusBarHeight - 1, bounds.width, bounds.height - (float)statusBarHeight + 1 };
            raylib::Rectangle closeButtonRec = { statusBar.x + statusBar.width - GuiGetStyle(GuiControl::STATUSBAR, GuiControlProperty::BORDER_WIDTH) - 20,
                                         statusBar.y + statusBarHeight/2.0f - 18.0f/2.0f, 18, 18 };

            // Update control
            //--------------------------------------------------------------------
            // NOTE: Logic is directly managed by button
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiStatusBar(statusBar, title); // Draw window header as status bar
            GuiPanel(windowPanel, NULL);    // Draw window base

            // Draw window close button
            int tempBorderWidth = GuiGetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH);
            int tempTextAlignment = GuiGetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, 1);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_ALIGNMENT, static_cast<int>(GuiTextAlignment::TEXT_ALIGN_CENTER));
        #if defined(RAYGUI_NO_ICONS)
            clicked = GuiButton(closeButtonRec, "x");
        #else
            clicked = GuiButton(closeButtonRec, GuiIconText(ICON_CROSS_SMALL, NULL));
        #endif
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, tempBorderWidth);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_ALIGNMENT, tempTextAlignment);
            //--------------------------------------------------------------------

            return clicked;
        }

        // Group Box control with text name
        void RayGui::GuiGroupBox(raylib::Rectangle bounds, const char *text)
        {
            #if !defined(RAYGUI_GROUPBOX_LINE_THICK)
                #define RAYGUI_GROUPBOX_LINE_THICK     1
            #endif

            GuiState state = guiState;

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(raylib::Rectangle{ bounds.x, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? GuiControlProperty::BORDER_COLOR_DISABLED : GuiDefaultProperty::LINE_COLOR)), guiAlpha));
            GuiDrawRectangle(raylib::Rectangle{ bounds.x, bounds.y + bounds.height - 1, bounds.width, RAYGUI_GROUPBOX_LINE_THICK }, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? GuiControlProperty::BORDER_COLOR_DISABLED : GuiDefaultProperty::LINE_COLOR)), guiAlpha));
            GuiDrawRectangle(raylib::Rectangle{ bounds.x + bounds.width - 1, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? GuiControlProperty::BORDER_COLOR_DISABLED : GuiDefaultProperty::LINE_COLOR)), guiAlpha));

            GuiLine(raylib::Rectangle{ bounds.x, bounds.y - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2, bounds.width, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE) }, text);
            //--------------------------------------------------------------------
        }

        // Line control
        void RayGui::GuiLine(raylib::Rectangle bounds, const char *text)
        {
            #if !defined(RAYGUI_LINE_ORIGIN_SIZE)
                #define RAYGUI_LINE_MARGIN_TEXT  12
            #endif
            #if !defined(RAYGUI_LINE_TEXT_PADDING)
                #define RAYGUI_LINE_TEXT_PADDING  4
            #endif

            GuiState state = guiState;

            raylib::Color color = Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? GuiControlProperty::BORDER_COLOR_DISABLED : GuiDefaultProperty::LINE_COLOR)), guiAlpha);

            // Draw control
            //--------------------------------------------------------------------
            if (text == NULL) GuiDrawRectangle(raylib::Rectangle{ bounds.x, bounds.y + bounds.height/2, bounds.width, 1 }, 0, raylib::BLANK, color);
            else
            {
                raylib::Rectangle textBounds = { 0 };
                textBounds.width = (float)GetTextWidth(text);
                textBounds.height = bounds.height;
                textBounds.x = bounds.x + RAYGUI_LINE_MARGIN_TEXT;
                textBounds.y = bounds.y;

                // Draw line with embedded text label: "--- text --------------"
                GuiDrawRectangle(raylib::Rectangle{ bounds.x, bounds.y + bounds.height/2, RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, raylib::BLANK, color);
                GuiDrawText(text, textBounds, TEXT_ALIGN_LEFT, color);
                GuiDrawRectangle(raylib::Rectangle{ bounds.x + 12 + textBounds.width + 4, bounds.y + bounds.height/2, bounds.width - textBounds.width - RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, raylib::BLANK, color);
            }
            //--------------------------------------------------------------------
        }

        // Panel control
        void RayGui::GuiPanel(raylib::Rectangle bounds, const char *text)
        {
            #if !defined(RAYGUI_PANEL_BORDER_WIDTH)
                #define RAYGUI_PANEL_BORDER_WIDTH   1
            #endif

            GuiState state = guiState;

            // Text will be drawn as a header bar (if provided)
            raylib::Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
            if ((text != NULL) && (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f)) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

            if (text != NULL)
            {
                // Move panel bounds after the header bar
                bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
                bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 1;
            }

            // Draw control
            //--------------------------------------------------------------------
            if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

            GuiDrawRectangle(bounds, RAYGUI_PANEL_BORDER_WIDTH, Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? GuiControlProperty::BORDER_COLOR_DISABLED: GuiDefaultProperty::LINE_COLOR)), guiAlpha),
                             Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state == GuiState::STATE_DISABLED)? BASE_COLOR_DISABLED : BACKGROUND_COLOR)), guiAlpha));
            //--------------------------------------------------------------------
        }

        // Scroll Panel control
        raylib::Rectangle RayGui::GuiScrollPanel(raylib::Rectangle bounds, const char *text, raylib::Rectangle content, raylib::Vector2 *scroll)
        {
            GuiState state = guiState;

            raylib::Vector2 scrollPos = { 0.0f, 0.0f };
            if (scroll != NULL) scrollPos = *scroll;

            // Text will be drawn as a header bar (if provided)
            raylib::Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
            if (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

            if (text != NULL)
            {
                // Move panel bounds after the header bar
                bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
                bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 1;
            }

            bool hasHorizontalScrollBar = (content.width > bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH))? true : false;
            bool hasVerticalScrollBar = (content.height > bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH))? true : false;

            // Recheck to account for the other scrollbar being visible
            if (!hasHorizontalScrollBar) hasHorizontalScrollBar = (hasVerticalScrollBar && (content.width > (bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH))))? true : false;
            if (!hasVerticalScrollBar) hasVerticalScrollBar = (hasHorizontalScrollBar && (content.height > (bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH))))? true : false;

            int horizontalScrollBarWidth = hasHorizontalScrollBar? GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH) : 0;
            int verticalScrollBarWidth =  hasVerticalScrollBar? GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH) : 0;
            raylib::Rectangle horizontalScrollBar = { (float)((GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + verticalScrollBarWidth : (float)bounds.x) + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), (float)bounds.y + bounds.height - horizontalScrollBarWidth - GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), (float)bounds.width - verticalScrollBarWidth - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), (float)horizontalScrollBarWidth };
            raylib::Rectangle verticalScrollBar = { (float)((GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) : (float)bounds.x + bounds.width - verticalScrollBarWidth - GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH)), (float)bounds.y + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), (float)verticalScrollBarWidth, (float)bounds.height - horizontalScrollBarWidth - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) };

            // Calculate view area (area without the scrollbars)
            raylib::Rectangle view = (GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)?
                        raylib::Rectangle{ bounds.x + verticalScrollBarWidth + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), bounds.y + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - horizontalScrollBarWidth } :
                        raylib::Rectangle{ bounds.x + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), bounds.y + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - horizontalScrollBarWidth };

            // Clip view area to the actual content size
            if (view.width > content.width) view.width = content.width;
            if (view.height > content.height) view.height = content.height;

            float horizontalMin = hasHorizontalScrollBar? ((GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) : (((float)GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);
            float horizontalMax = hasHorizontalScrollBar? content.width - bounds.width + (float)verticalScrollBarWidth + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - (((float)GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)verticalScrollBarWidth : 0) : (float)-GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);
            float verticalMin = hasVerticalScrollBar? 0 : -1;
            float verticalMax = hasVerticalScrollBar? content.height - bounds.height + (float)horizontalScrollBarWidth + (float)GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) : (float)-GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check button state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;

        #if defined(SUPPORT_SCROLLBAR_KEY_INPUT)
                    if (hasHorizontalScrollBar)
                    {
                        if (IsKeyDown(KEY_RIGHT)) scrollPos.x -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                        if (IsKeyDown(KEY_LEFT)) scrollPos.x += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                    }

                    if (hasVerticalScrollBar)
                    {
                        if (IsKeyDown(KEY_DOWN)) scrollPos.y -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                        if (IsKeyDown(KEY_UP)) scrollPos.y += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                    }
        #endif
                    float wheelMove = raylib::GetMouseWheelMove();

                    // Horizontal scroll (Shift + Mouse wheel)
                    if (hasHorizontalScrollBar && (raylib::IsKeyDown(raylib::KEY_LEFT_CONTROL) || raylib::IsKeyDown(raylib::KEY_RIGHT_SHIFT))) scrollPos.x += wheelMove*20;
                    else scrollPos.y += wheelMove*20; // Vertical scroll
                }
            }

            // Normalize scroll values
            if (scrollPos.x > -horizontalMin) scrollPos.x = -horizontalMin;
            if (scrollPos.x < -horizontalMax) scrollPos.x = -horizontalMax;
            if (scrollPos.y > -verticalMin) scrollPos.y = -verticalMin;
            if (scrollPos.y < -verticalMax) scrollPos.y = -verticalMax;
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

            GuiDrawRectangle(bounds, 0, raylib::BLANK, raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, BACKGROUND_COLOR)));        // Draw background

            // Save size of the scrollbar slider
            const int slider = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);

            // Draw horizontal scrollbar if visible
            if (hasHorizontalScrollBar)
            {
                // Change scrollbar slider size to show the diff in size between the content width and the widget width
                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - verticalScrollBarWidth)/(int)content.width)*((int)bounds.width - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - verticalScrollBarWidth)));
                scrollPos.x = (float)-GuiScrollBar(horizontalScrollBar, (int)-scrollPos.x, (int)horizontalMin, (int)horizontalMax);
            }
            else scrollPos.x = 0.0f;

            // Draw vertical scrollbar if visible
            if (hasVerticalScrollBar)
            {
                // Change scrollbar slider size to show the diff in size between the content height and the widget height
                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - horizontalScrollBarWidth)/(int)content.height)*((int)bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) - horizontalScrollBarWidth)));
                scrollPos.y = (float)-GuiScrollBar(verticalScrollBar, (int)-scrollPos.y, (int)verticalMin, (int)verticalMax);
            }
            else scrollPos.y = 0.0f;

            // Draw detail corner raylib::Rectangle if both scroll bars are visible
            if (hasHorizontalScrollBar && hasVerticalScrollBar)
            {
                raylib::Rectangle corner = { (GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (bounds.x + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH) + 2) : (horizontalScrollBar.x + horizontalScrollBar.width + 2), verticalScrollBar.y + verticalScrollBar.height + 2, (float)horizontalScrollBarWidth - 4, (float)verticalScrollBarWidth - 4 };
                GuiDrawRectangle(corner, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, TEXT + (state*3))), guiAlpha));
            }

            // Draw scrollbar lines depending on current state
            GuiDrawRectangle(bounds, GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BORDER + (state*3))), guiAlpha), raylib::BLANK);

            // Set scrollbar slider size back to the way it was before
            GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, slider);
            //--------------------------------------------------------------------

            if (scroll != NULL) *scroll = scrollPos;

            return view;
        }

        // Label control
        void RayGui::GuiLabel(raylib::Rectangle bounds, const char *text)
        {
            GuiState state = guiState;

            // Update control
            //--------------------------------------------------------------------
            // ...
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(LABEL, TEXT + (state*3))), guiAlpha));
            //--------------------------------------------------------------------
        }

        // Button control, returns true when clicked
        bool RayGui::GuiButton(raylib::Rectangle bounds, const char *text)
        {
            GuiState state = guiState;
            bool pressed = false;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check button state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;

                    if (raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(BUTTON, BASE + (state*3))), guiAlpha));
            GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))), guiAlpha));
            //------------------------------------------------------------------

            return pressed;
        }

        // Label button control
        bool RayGui::GuiLabelButton(raylib::Rectangle bounds, const char *text)
        {
            GuiState state = guiState;
            bool pressed = false;

            // NOTE: We force bounds.width to be all text
            float textWidth = MeasureTextEx(guiFont, text, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING)).x;
            if (bounds.width < textWidth) bounds.width = textWidth;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check checkbox state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;

                    if (raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(LABEL, TEXT + (state*3))), guiAlpha));
            //--------------------------------------------------------------------

            return pressed;
        }

        // Toggle Button control, returns true when active
        bool RayGui::GuiToggle(raylib::Rectangle bounds, const char *text, bool active)
        {
            GuiState state = guiState;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check toggle button state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else if (raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON))
                    {
                        state = STATE_NORMAL;
                        active = !active;
                    }
                    else state = STATE_FOCUSED;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (state == STATE_NORMAL)
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, (active? BORDER_COLOR_PRESSED : (BORDER + state*3)))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, (active? BASE_COLOR_PRESSED : (BASE + state*3)))), guiAlpha));
                GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, (active? TEXT_COLOR_PRESSED : (TEXT + state*3)))), guiAlpha));
            }
            else
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, BORDER + state*3)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, BASE + state*3)), guiAlpha));
                GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(TOGGLE, TEXT + state*3)), guiAlpha));
            }
            //--------------------------------------------------------------------

            return active;
        }

        // Toggle Group control, returns toggled button index
        int RayGui::GuiToggleGroup(raylib::Rectangle bounds, const char *text, int active)
        {
            #if !defined(RAYGUI_TOGGLEGROUP_MAX_ITEMS)
                #define RAYGUI_TOGGLEGROUP_MAX_ITEMS    32
            #endif

            float initBoundsX = bounds.x;

            // Get substrings items from text (items pointers)
            int rows[RAYGUI_TOGGLEGROUP_MAX_ITEMS] = { 0 };
            int itemCount = 0;
            const char **items = GuiTextSplit(text, &itemCount, rows);

            int prevRow = rows[0];

            for (int i = 0; i < itemCount; i++)
            {
                if (prevRow != rows[i])
                {
                    bounds.x = initBoundsX;
                    bounds.y += (bounds.height + GuiGetStyle(TOGGLE, GROUP_PADDING));
                    prevRow = rows[i];
                }

                if (i == active) GuiToggle(bounds, items[i], true);
                else if (GuiToggle(bounds, items[i], false) == true) active = i;

                bounds.x += (bounds.width + GuiGetStyle(TOGGLE, GROUP_PADDING));
            }

            return active;
        }

        // Check Box control, returns true when active
        bool RayGui::GuiCheckBox(raylib::Rectangle bounds, const char *text, bool checked)
        {
            GuiState state = guiState;

            raylib::Rectangle textBounds = { 0 };

            if (text != NULL)
            {
                textBounds.width = (float)GetTextWidth(text);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x + bounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;
                if (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(CHECKBOX, TEXT_PADDING);
            }

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                raylib::Rectangle totalBounds = {
                    (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT)? textBounds.x : bounds.x,
                    bounds.y,
                    bounds.width + textBounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING),
                    bounds.height,
                };

                // Check checkbox state
                if (CheckCollisionPointRec(mousePoint, totalBounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;

                    if (raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON)) checked = !checked;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(CHECKBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(CHECKBOX, BORDER + (state*3))), guiAlpha), raylib::BLANK);

            if (checked)
            {
                raylib::Rectangle check = { bounds.x + GuiGetStyle(CHECKBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                                    bounds.y + GuiGetStyle(CHECKBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                                    bounds.width - 2*(GuiGetStyle(CHECKBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)),
                                    bounds.height - 2*(GuiGetStyle(CHECKBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)) };
                GuiDrawRectangle(check, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(CHECKBOX, TEXT + state*3)), guiAlpha));
            }

            GuiDrawText(text, textBounds, (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, Fade(raylib::GetColor(GuiGetStyle(LABEL, TEXT + (state*3))), guiAlpha));
            //--------------------------------------------------------------------

            return checked;
        }

        // Combo Box control, returns selected item index
        int RayGui::GuiComboBox(raylib::Rectangle bounds, const char *text, int active)
        {
            GuiState state = guiState;

            bounds.width -= (GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH) + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING));

            raylib::Rectangle selector = { (float)bounds.x + bounds.width + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING),
                                   (float)bounds.y, (float)GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH), (float)bounds.height };

            // Get substrings items from text (items pointers, lengths and count)
            int itemCount = 0;
            const char **items = GuiTextSplit(text, &itemCount, NULL);

            if (active < 0) active = 0;
            else if (active > itemCount - 1) active = itemCount - 1;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked && (itemCount > 1))
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds) ||
                    CheckCollisionPointRec(mousePoint, selector))
                {
                    if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))
                    {
                        active += 1;
                        if (active >= itemCount) active = 0;
                    }

                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            // Draw combo box main
            GuiDrawRectangle(bounds, GuiGetStyle(COMBOBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(COMBOBOX, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(COMBOBOX, BASE + (state*3))), guiAlpha));
            GuiDrawText(items[active], GetTextBounds(COMBOBOX, bounds), GuiGetStyle(COMBOBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(COMBOBOX, TEXT + (state*3))), guiAlpha));

            // Draw selector using a custom button
            // NOTE: GuiControlProperty::BORDER_WIDTH and TEXT_ALIGNMENT forced values
            int tempBorderWidth = GuiGetStyle(BUTTON, GuiControlProperty::BORDER_WIDTH);
            int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, 1);
            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

            GuiButton(selector, raylib::TextFormat("%i/%i", active + 1, itemCount));

            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, tempTextAlign);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, tempBorderWidth);
            //--------------------------------------------------------------------

            return active;
        }

        // Dropdown Box control
        // NOTE: Returns mouse click
        bool RayGui::GuiDropdownBox(raylib::Rectangle bounds, const char *text, int *active, bool editMode)
        {
            GuiState state = guiState;
            int itemSelected = *active;
            int itemFocused = -1;

            // Get substrings items from text (items pointers, lengths and count)
            int itemCount = 0;
            const char **items = GuiTextSplit(text, &itemCount, NULL);

            raylib::Rectangle boundsOpen = bounds;
            boundsOpen.height = (itemCount + 1)*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

            raylib::Rectangle itemBounds = bounds;

            bool pressed = false;       // Check mouse button pressed

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && (editMode || !guiLocked) && (itemCount > 1))
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (editMode)
                {
                    state = STATE_PRESSED;

                    // Check if mouse has been pressed or released outside limits
                    if (!CheckCollisionPointRec(mousePoint, boundsOpen))
                    {
                        if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) || raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                    }

                    // Check if already selected item has been pressed again
                    if (CheckCollisionPointRec(mousePoint, bounds) && raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) pressed = true;

                    // Check focused and selected item
                    for (int i = 0; i < itemCount; i++)
                    {
                        // Update item raylib::Rectangle y position for next item
                        itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

                        if (CheckCollisionPointRec(mousePoint, itemBounds))
                        {
                            itemFocused = i;
                            if (raylib::IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON))
                            {
                                itemSelected = i;
                                pressed = true;     // Item selected, change to editMode = false
                            }
                            break;
                        }
                    }

                    itemBounds = bounds;
                }
                else
                {
                    if (CheckCollisionPointRec(mousePoint, bounds))
                    {
                        if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))
                        {
                            pressed = true;
                            state = STATE_PRESSED;
                        }
                        else state = STATE_FOCUSED;
                    }
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (editMode) GuiPanel(boundsOpen, NULL);

            GuiDrawRectangle(bounds, GuiGetStyle(DROPDOWNBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state*3)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state*3)), guiAlpha));
            GuiDrawText(items[itemSelected], GetTextBounds(GuiControl::DEFAULT, bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + state*3)), guiAlpha));

            if (editMode)
            {
                // Draw visible items
                for (int i = 0; i < itemCount; i++)
                {
                    // Update item raylib::Rectangle y position for next item
                    itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

                    if (i == itemSelected)
                    {
                        GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED)), guiAlpha));
                        GuiDrawText(items[i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED)), guiAlpha));
                    }
                    else if (i == itemFocused)
                    {
                        GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)), guiAlpha));
                        GuiDrawText(items[i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)), guiAlpha));
                    }
                    else GuiDrawText(items[i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)), guiAlpha));
                }
            }

            // Draw arrows (using icon if available)
        #if defined(RAYGUI_NO_ICONS)
            GuiDrawText("v", raylib::Rectangle{ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 2, 10, 10 },
                        TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))), guiAlpha));
        #else
            GuiDrawText("#120#", raylib::Rectangle{ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 6, 10, 10 },
                        TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))), guiAlpha));   // ICON_ARROW_DOWN_FILL
        #endif
            //--------------------------------------------------------------------

            *active = itemSelected;
            return pressed;
        }

        // Text Box control, updates input text
        // NOTE 2: Returns if KEY_ENTER pressed (useful for data validation)
        bool RayGui::GuiTextBox(raylib::Rectangle bounds, char *text, int textSize, bool editMode)
        {
            GuiState state = guiState;
            bool pressed = false;

            raylib::Rectangle cursor = {
                bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(text) + 2,
                bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE),
                4,
                (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)*2
            };

            if (cursor.height >= bounds.height) cursor.height = bounds.height - GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH)*2;
            if (cursor.y < (bounds.y + GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH))) cursor.y = bounds.y + GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH);

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (editMode)
                {
                    state = STATE_PRESSED;

                    int key = raylib::GetCharPressed();      // Returns codepoint as Unicode

                    int keyCount = std::string(text).size();
                    int byteSize = 0;
                    const char *textUTF8 = raylib::CodepointToUTF8(key, &byteSize);

                    // Only allow keys in range [32..125]
                    if ((keyCount + byteSize) < textSize)
                    {
                        float maxWidth = (bounds.width - (GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING)*2));

                        if ((GetTextWidth(text) < (maxWidth - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE))) && (key >= 32))
                        {
                            for (int i = 0; i < byteSize; i++)
                            {
                                text[keyCount] = textUTF8[i];
                                keyCount++;
                            }

                            text[keyCount] = '\0';
                        }
                    }

                    // Delete text
                    if (keyCount > 0)
                    {
                        if (raylib::IsKeyPressed(raylib::KEY_BACKSPACE))
                        {
                            while ((keyCount > 0) && ((text[--keyCount] & 0xc0) == 0x80));
                            text[keyCount] = '\0';
                        }
                    }

                    if (raylib::IsKeyPressed(raylib::KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, bounds) && raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))) pressed = true;

                    // Check text alignment to position cursor properly
                    int textAlignment = GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT);
                    if (textAlignment == TEXT_ALIGN_CENTER) cursor.x = bounds.x + GetTextWidth(text)/2 + bounds.width/2 + 1;
                    else if (textAlignment == TEXT_ALIGN_RIGHT) cursor.x = bounds.x + bounds.width - GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING);
                }
                else
                {
                    if (CheckCollisionPointRec(mousePoint, bounds))
                    {
                        state = STATE_FOCUSED;
                        if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                    }
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (state == STATE_PRESSED)
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha));
            }
            else if (state == GuiState::STATE_DISABLED)
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)), guiAlpha));
            }
            else GuiDrawRectangle(bounds, 1, Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), raylib::BLANK);

            GuiDrawText(text, GetTextBounds(TEXTBOX, bounds), GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, TEXT + (state*3))), guiAlpha));

            // Draw cursor
            if (editMode) GuiDrawRectangle(cursor, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha));
            //--------------------------------------------------------------------

            return pressed;
        }

        // Spinner control, returns selected value
        bool RayGui::GuiSpinner(raylib::Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
        {
            GuiState state = guiState;

            bool pressed = false;
            int tempValue = *value;

            raylib::Rectangle spinner = { bounds.x + GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING), bounds.y,
                                  bounds.width - 2*(GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING)), bounds.height };
            raylib::Rectangle leftButtonBound = { (float)bounds.x, (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };
            raylib::Rectangle rightButtonBound = { (float)bounds.x + bounds.width - GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };

            raylib::Rectangle textBounds = { 0 };
            if (text != NULL)
            {
                textBounds.width = (float)GetTextWidth(text);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x + bounds.width + GuiGetStyle(SPINNER, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;
                if (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SPINNER, TEXT_PADDING);
            }

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check spinner state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;
                }
            }

        #if defined(RAYGUI_NO_ICONS)
            if (GuiButton(leftButtonBound, "<")) tempValue--;
            if (GuiButton(rightButtonBound, ">")) tempValue++;
        #else
            if (GuiButton(leftButtonBound, GuiIconText(ICON_ARROW_LEFT_FILL, NULL))) tempValue--;
            if (GuiButton(rightButtonBound, GuiIconText(ICON_ARROW_RIGHT_FILL, NULL))) tempValue++;
        #endif

            if (!editMode)
            {
                if (tempValue < minValue) tempValue = minValue;
                if (tempValue > maxValue) tempValue = maxValue;
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            // TODO: Set Spinner properties for ValueBox
            pressed = GuiValueBox(spinner, NULL, &tempValue, minValue, maxValue, editMode);

            // Draw value selector custom buttons
            // NOTE: GuiControlProperty::BORDER_WIDTH and TEXT_ALIGNMENT forced values
            int tempBorderWidth = GuiGetStyle(BUTTON, GuiControlProperty::BORDER_WIDTH);
            int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, GuiGetStyle(SPINNER, GuiControlProperty::BORDER_WIDTH));
            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, tempTextAlign);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, tempBorderWidth);

            // Draw text label if provided
            GuiDrawText(text, textBounds, (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, Fade(raylib::GetColor(GuiGetStyle(LABEL, TEXT + (state*3))), guiAlpha));
            //--------------------------------------------------------------------

            *value = tempValue;
            return pressed;
        }

        // Value Box control, updates input text with numbers
        // NOTE: Requires static variables: frameCounter
        bool RayGui::GuiValueBox(raylib::Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
        {

            #if !defined(RAYGUI_VALUEBOX_MAX_CHARS)
                #define RAYGUI_VALUEBOX_MAX_CHARS  32
            #endif

            GuiState state = guiState;
            bool pressed = false;

            char textValue[RAYGUI_VALUEBOX_MAX_CHARS + 1] = "\0";
            sprintf(textValue, "%i", *value);

            raylib::Rectangle textBounds = { 0 };
            if (text != NULL)
            {
                textBounds.width = (float)GetTextWidth(text);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x + bounds.width + GuiGetStyle(VALUEBOX, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;
                if (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(VALUEBOX, TEXT_PADDING);
            }

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                bool valueHasChanged = false;

                if (editMode)
                {
                    state = STATE_PRESSED;

                    int keyCount = std::string(textValue).size();;

                    // Only allow keys in range [48..57]
                    if (keyCount < RAYGUI_VALUEBOX_MAX_CHARS)
                    {
                        if (GetTextWidth(textValue) < bounds.width)
                        {
                            int key = raylib::GetCharPressed();
                            if ((key >= 48) && (key <= 57))
                            {
                                textValue[keyCount] = (char)key;
                                keyCount++;
                                valueHasChanged = true;
                            }
                        }
                    }

                    // Delete text
                    if (keyCount > 0)
                    {
                        if (raylib::IsKeyPressed(raylib::KEY_BACKSPACE))
                        {
                            keyCount--;
                            textValue[keyCount] = '\0';
                            valueHasChanged = true;
                        }
                    }

                    if (valueHasChanged) *value = raylib::TextToInteger(textValue);

                    // NOTE: We are not clamp values until user input finishes
                    //if (*value > maxValue) *value = maxValue;
                    //else if (*value < minValue) *value = minValue;

                    if (raylib::IsKeyPressed(raylib::KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, bounds) && raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))) pressed = true;
                }
                else
                {
                    if (*value > maxValue) *value = maxValue;
                    else if (*value < minValue) *value = minValue;

                    if (CheckCollisionPointRec(mousePoint, bounds))
                    {
                        state = STATE_FOCUSED;
                        if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                    }
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            raylib::Color baseColor = raylib::BLANK;
            if (state == STATE_PRESSED) baseColor = raylib::GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED));
            else if (state == GuiState::STATE_DISABLED) baseColor = raylib::GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_DISABLED));

            // WARNING: raylib::BLANK color does not work properly with Fade()
            GuiDrawRectangle(bounds, GuiGetStyle(VALUEBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(VALUEBOX, BORDER + (state*3))), guiAlpha), baseColor);
            GuiDrawText(textValue, GetTextBounds(VALUEBOX, bounds), TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(VALUEBOX, TEXT + (state*3))), guiAlpha));

            // Draw cursor
            if (editMode)
            {
                // NOTE: ValueBox internal text is always centered
                raylib::Rectangle cursor = { bounds.x + GetTextWidth(textValue)/2 + bounds.width/2 + 2, bounds.y + 2*GuiGetStyle(VALUEBOX, GuiControlProperty::BORDER_WIDTH), 4, bounds.height - 4*GuiGetStyle(VALUEBOX, GuiControlProperty::BORDER_WIDTH) };
                GuiDrawRectangle(cursor, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED)), guiAlpha));
            }

            // Draw text label if provided
            GuiDrawText(text, textBounds, (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, Fade(raylib::GetColor(GuiGetStyle(LABEL, TEXT + (state*3))), guiAlpha));
            //--------------------------------------------------------------------

            return pressed;

        }

        // Text Box control with multiple lines
        bool RayGui::GuiTextBoxMulti(raylib::Rectangle bounds, char *text, int textSize, bool editMode)
        {
            GuiState state = guiState;
            bool pressed = false;

            raylib::Rectangle textAreaBounds = {
                bounds.x + GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING),
                bounds.y + GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING),
                bounds.width - 2*(GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING)),
                bounds.height - 2*(GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING))
            };

            // Cursor position, [x, y] values should be updated
            raylib::Rectangle cursor = { 0, -1, 4, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE) + 2 };

            float scaleFactor = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/(float)guiFont.baseSize;     // Character raylib::Rectangle scaling factor

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (editMode)
                {
                    state = STATE_PRESSED;

                    // We get an Unicode codepoint
                    int codepoint = raylib::GetCharPressed();
                    int textLength = std::string(text).size();     // Length in bytes (UTF-8 string)

                    // Introduce characters
                    if (textLength < (textSize - 1))
                    {
                        if (raylib::IsKeyPressed(raylib::KEY_ENTER))
                        {
                            text[textLength] = '\n';
                            textLength++;
                        }
                        else if (codepoint >= 32)
                        {
                            // Supports Unicode inputs -> Encoded to UTF-8
                            int charUTF8Length = 0;
                            const char *charEncoded = raylib::CodepointToUTF8(codepoint, &charUTF8Length);
                            memcpy(text + textLength, charEncoded, charUTF8Length);
                            textLength += charUTF8Length;
                        }
                    }

                    // Delete characters
                    if (textLength > 0)
                    {
                        if (raylib::IsKeyPressed(raylib::KEY_BACKSPACE))
                        {
                            if ((unsigned char)text[textLength - 1] < 127)
                            {
                                // Remove ASCII equivalent character (1 byte)
                                textLength--;
                                text[textLength] = '\0';
                            }
                            else
                            {
                                // Remove latest UTF-8 unicode character introduced (n bytes)
                                int charUTF8Length = 0;
                                while (((unsigned char)text[textLength - 1 - charUTF8Length] & 0b01000000) == 0) charUTF8Length++;

                                textLength -= (charUTF8Length + 1);
                                text[textLength] = '\0';
                            }
                        }
                    }

                    // Exit edit mode
                    if (!CheckCollisionPointRec(mousePoint, bounds) && raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                }
                else
                {
                    if (CheckCollisionPointRec(mousePoint, bounds))
                    {
                        state = STATE_FOCUSED;
                        if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) pressed = true;
                    }
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (state == STATE_PRESSED)
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha));
            }
            else if (state == GuiState::STATE_DISABLED)
            {
                GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)), guiAlpha));
            }
            else GuiDrawRectangle(bounds, 1, Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha), raylib::BLANK);

            int wrapMode = 1;      // 0-No wrap, 1-Char wrap, 2-Word wrap
            raylib::Vector2 cursorPos = { textAreaBounds.x, textAreaBounds.y };

            //int lastSpacePos = 0;
            //int lastSpaceWidth = 0;
            //int lastSpaceCursorPos = 0;

            for (int i = 0, codepointLength = 0; text[i] != '\0'; i += codepointLength)
            {
                int codepoint = raylib::GetCodepoint(text + i, &codepointLength);
                int index = GetGlyphIndex(guiFont, codepoint);      // If requested codepoint is not found, we get '?' (0x3f)
                raylib::Rectangle atlasRec = guiFont.recs[index];
                raylib::GlyphInfo glyphInfo = guiFont.glyphs[index];        // Glyph measures

                if ((codepointLength == 1) && (codepoint == '\n'))
                {
                    cursorPos.y += (guiFont.baseSize*scaleFactor + GuiGetStyle(TEXTBOX, TEXT_LINES_SPACING));   // Line feed
                    cursorPos.x = textAreaBounds.x;                 // Carriage return
                }
                else
                {
                    if (wrapMode == 1)
                    {
                        int glyphWidth = 0;
                        if (glyphInfo.advanceX != 0) glyphWidth += glyphInfo.advanceX;
                        else glyphWidth += (int)(atlasRec.width + glyphInfo.offsetX);

                        // Jump line if the end of the text box area has been reached
                        if ((cursorPos.x + (glyphWidth*scaleFactor)) > (textAreaBounds.x + textAreaBounds.width))
                        {
                            cursorPos.y += (guiFont.baseSize*scaleFactor + GuiGetStyle(TEXTBOX, TEXT_LINES_SPACING));   // Line feed
                            cursorPos.x = textAreaBounds.x;     // Carriage return
                        }
                    }
                    else if (wrapMode == 2)
                    {
                        /*
                        if ((codepointLength == 1) && (codepoint == ' '))
                        {
                            lastSpacePos = i;
                            lastSpaceWidth = 0;
                            lastSpaceCursorPos = cursorPos.x;
                        }

                        // Jump line if last word reaches end of text box area
                        if ((lastSpaceCursorPos + lastSpaceWidth) > (textAreaBounds.x + textAreaBounds.width))
                        {
                            cursorPos.y += 12;               // Line feed
                            cursorPos.x = textAreaBounds.x;  // Carriage return
                        }
                        */
                    }

                    // Draw current character glyph
                    DrawTextCodepoint(guiFont, codepoint, cursorPos, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, TEXT + (state*3))), guiAlpha));

                    int glyphWidth = 0;
                    if (glyphInfo.advanceX != 0) glyphWidth += glyphInfo.advanceX;
                    else glyphWidth += (int)(atlasRec.width + glyphInfo.offsetX);

                    cursorPos.x += (glyphWidth*scaleFactor + (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING));
                    //if (i > lastSpacePos) lastSpaceWidth += (atlasRec.width + (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING));
                }
            }

            cursor.x = cursorPos.x;
            cursor.y = cursorPos.y;

            // Draw cursor position considering text glyphs
            if (editMode) GuiDrawRectangle(cursor, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha));
            //--------------------------------------------------------------------

            return pressed;
        }

        // Slider control with pro parameters
        // NOTE: Other GuiSlider*() controls use this one
        float RayGui::GuiSliderPro(raylib::Rectangle bounds, const char *textLeft, const char *textRight, float value, float minValue, float maxValue, int sliderWidth)
        {

            RayGui::GuiState state = guiState;

            int sliderValue = (int)(((value - minValue)/(maxValue - minValue))*(bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH)));

            raylib::Rectangle slider = { bounds.x, bounds.y + GuiGetStyle(SLIDER, BORDER_WIDTH) + GuiGetStyle(SLIDER, SLIDER_PADDING),
                                 0, bounds.height - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - 2*GuiGetStyle(SLIDER, SLIDER_PADDING) };

            if (sliderWidth > 0)        // Slider
            {
                slider.x += (sliderValue - sliderWidth/2);
                slider.width = (float)sliderWidth;
            }
            else if (sliderWidth == 0)  // SliderBar
            {
                slider.x += GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH);
                slider.width = (float)sliderValue;
            }

            // Update control
            //--------------------------------------------------------------------
            if ((state != STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON))
                    {
                        state = STATE_PRESSED;

                        // Get equivalent value and slider position from mousePoint.x
                        value = ((maxValue - minValue)*(mousePoint.x - (float)(bounds.x + sliderWidth/2)))/(float)(bounds.width - sliderWidth) + minValue;

                        if (sliderWidth > 0) slider.x = mousePoint.x - slider.width/2;  // Slider
                        else if (sliderWidth == 0) slider.width = (float)sliderValue;          // SliderBar
                    }
                    else state = STATE_FOCUSED;
                }

                if (value > maxValue) value = maxValue;
                else if (value < minValue) value = minValue;
            }

            // Bar limits check
            if (sliderWidth > 0)        // Slider
            {
                if (slider.x <= (bounds.x + GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH))) slider.x = bounds.x + GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH);
                else if ((slider.x + slider.width) >= (bounds.x + bounds.width)) slider.x = bounds.x + bounds.width - slider.width - GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH);
            }
            else if (sliderWidth == 0)  // SliderBar
            {
                if (slider.width > bounds.width) slider.width = bounds.width - 2*GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH);
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(SLIDER, BORDER + (state*3))), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(SLIDER, (state != GuiState::STATE_DISABLED)?  BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)), guiAlpha));

            // Draw slider internal bar (depends on state)
            if ((state == STATE_NORMAL) || (state == STATE_PRESSED)) GuiDrawRectangle(slider, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)), guiAlpha));
            else if (state == STATE_FOCUSED) GuiDrawRectangle(slider, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_FOCUSED)), guiAlpha));

            // Draw left/right text if provided
            if (textLeft != NULL)
            {
                raylib::Rectangle textBounds = { 0 };
                textBounds.width = (float)GetTextWidth(textLeft);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SLIDER, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;

                GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, Fade(raylib::GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))), guiAlpha));
            }

            if (textRight != NULL)
            {
                raylib::Rectangle textBounds = { 0 };
                textBounds.width = (float)GetTextWidth(textRight);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x + bounds.width + GuiGetStyle(SLIDER, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;

                GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, Fade(raylib::GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))), guiAlpha));
            }
            //--------------------------------------------------------------------

            return value;
        }

        // Slider control extended, returns selected value and has text
        float RayGui::GuiSlider(raylib::Rectangle bounds, const char *textLeft, const char *textRight, float value, float minValue, float maxValue)
        {
            return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, GuiGetStyle(SLIDER, SLIDER_WIDTH));
        }

        // Slider Bar control extended, returns selected value
        float RayGui::GuiSliderBar(raylib::Rectangle bounds, const char *textLeft, const char *textRight, float value, float minValue, float maxValue)
        {
            return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, 0);
        }

        // Progress Bar control extended, shows current progress value
        float RayGui::GuiProgressBar(raylib::Rectangle bounds, const char *textLeft, const char *textRight, float value, float minValue, float maxValue)
        {
            GuiState state = guiState;

            raylib::Rectangle progress = { bounds.x + GuiGetStyle(PROGRESSBAR, GuiControlProperty::BORDER_WIDTH),
                                   bounds.y + GuiGetStyle(PROGRESSBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING), 0,
                                   bounds.height - 2*GuiGetStyle(PROGRESSBAR, GuiControlProperty::BORDER_WIDTH) - 2*GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING) };

            // Update control
            //--------------------------------------------------------------------
            if (value > maxValue) value = maxValue;

            if (state != GuiState::STATE_DISABLED) progress.width = ((float)(value/(maxValue - minValue))*(float)(bounds.width - 2*GuiGetStyle(PROGRESSBAR, GuiControlProperty::BORDER_WIDTH)));
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(PROGRESSBAR, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(PROGRESSBAR, BORDER + (state*3))), guiAlpha), raylib::BLANK);

            // Draw slider internal progress bar (depends on state)
            if ((state == STATE_NORMAL) || (state == STATE_PRESSED)) GuiDrawRectangle(progress, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(PROGRESSBAR, BASE_COLOR_PRESSED)), guiAlpha));
            else if (state == STATE_FOCUSED) GuiDrawRectangle(progress, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(PROGRESSBAR, TEXT_COLOR_FOCUSED)), guiAlpha));

            // Draw left/right text if provided
            if (textLeft != NULL)
            {
                raylib::Rectangle textBounds = { 0 };
                textBounds.width = (float)GetTextWidth(textLeft);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x - textBounds.width - GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;

                GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, Fade(raylib::GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))), guiAlpha));
            }

            if (textRight != NULL)
            {
                raylib::Rectangle textBounds = { 0 };
                textBounds.width = (float)GetTextWidth(textRight);
                textBounds.height = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
                textBounds.x = bounds.x + bounds.width + GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
                textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE)/2;

                GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, Fade(raylib::GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))), guiAlpha));
            }
            //--------------------------------------------------------------------

            return value;
        }

        // Status Bar control
        void RayGui::GuiStatusBar(raylib::Rectangle bounds, const char *text)
        {
            GuiState state = guiState;

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(STATUSBAR, (state != GuiState::STATE_DISABLED)? BORDER_COLOR_NORMAL : GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha),
                             Fade(raylib::GetColor(GuiGetStyle(STATUSBAR, (state != GuiState::STATE_DISABLED)? BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)), guiAlpha));
            GuiDrawText(text, GetTextBounds(STATUSBAR, bounds), GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(STATUSBAR, (state != GuiState::STATE_DISABLED)? TEXT_COLOR_NORMAL : TEXT_COLOR_DISABLED)), guiAlpha));
            //--------------------------------------------------------------------
        }

        // Dummy raylib::Rectangle control, intended for placeholding
        void RayGui::GuiDummyRec(raylib::Rectangle bounds, const char *text)
        {
            GuiState state = guiState;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check button state
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
                    else state = STATE_FOCUSED;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, (state != GuiState::STATE_DISABLED)? BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)), guiAlpha));
            GuiDrawText(text, GetTextBounds(GuiControl::DEFAULT, bounds), TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(BUTTON, (state != GuiState::STATE_DISABLED)? TEXT_COLOR_NORMAL : TEXT_COLOR_DISABLED)), guiAlpha));
            //------------------------------------------------------------------
        }

        // List View control
        int RayGui::GuiListView(raylib::Rectangle bounds, const char *text, int *scrollIndex, int active)
        {
            int itemCount = 0;
            const char **items = NULL;

            if (text != NULL) items = GuiTextSplit(text, &itemCount, NULL);

            return GuiListViewEx(bounds, items, itemCount, NULL, scrollIndex, active);
        }

        // List View control with extended parameters
        int RayGui::GuiListViewEx(raylib::Rectangle bounds, const char **text, int count, int *focus, int *scrollIndex, int active)
        {
            GuiState state = guiState;
            int itemFocused = (focus == NULL)? -1 : *focus;
            int itemSelected = active;

            // Check if we need a scroll bar
            bool useScrollBar = false;
            if ((GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING))*count > bounds.height) useScrollBar = true;

            // Define base item raylib::Rectangle [0]
            raylib::Rectangle itemBounds = { 0 };
            itemBounds.x = bounds.x + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING);
            itemBounds.y = bounds.y + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);
            itemBounds.width = bounds.width - 2*GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING) - GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);
            itemBounds.height = (float)GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT);
            if (useScrollBar) itemBounds.width -= GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH);

            // Get items on the list
            int visibleItems = (int)bounds.height/(GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING));
            if (visibleItems > count) visibleItems = count;

            int startIndex = (scrollIndex == NULL)? 0 : *scrollIndex;
            if ((startIndex < 0) || (startIndex > (count - visibleItems))) startIndex = 0;
            int endIndex = startIndex + visibleItems;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                // Check mouse inside list view
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    state = STATE_FOCUSED;

                    // Check focused and selected item
                    for (int i = 0; i < visibleItems; i++)
                    {
                        if (CheckCollisionPointRec(mousePoint, itemBounds))
                        {
                            itemFocused = startIndex + i;
                            if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))
                            {
                                if (itemSelected == (startIndex + i)) itemSelected = -1;
                                else itemSelected = startIndex + i;
                            }
                            break;
                        }

                        // Update item raylib::Rectangle y position for next item
                        itemBounds.y += (GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING));
                    }

                    if (useScrollBar)
                    {
                        int wheelMove = (int)raylib::GetMouseWheelMove();
                        startIndex -= wheelMove;

                        if (startIndex < 0) startIndex = 0;
                        else if (startIndex > (count - visibleItems)) startIndex = count - visibleItems;

                        endIndex = startIndex + visibleItems;
                        if (endIndex > count) endIndex = count;
                    }
                }
                else itemFocused = -1;

                // Reset item raylib::Rectangle y to [0]
                itemBounds.y = bounds.y + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH);
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BORDER + state*3)), guiAlpha), raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, BACKGROUND_COLOR)));     // Draw background

            // Draw visible items
            for (int i = 0; ((i < visibleItems) && (text != NULL)); i++)
            {
                if (state == GuiState::STATE_DISABLED)
                {
                    if ((startIndex + i) == itemSelected) GuiDrawRectangle(itemBounds, GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BASE_COLOR_DISABLED)), guiAlpha));

                    GuiDrawText(text[startIndex + i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(GuiControl::LISTVIEW, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, TEXT_COLOR_DISABLED)), guiAlpha));
                }
                else
                {
                    if ((startIndex + i) == itemSelected)
                    {
                        // Draw item selected
                        GuiDrawRectangle(itemBounds, GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BORDER_COLOR_PRESSED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BASE_COLOR_PRESSED)), guiAlpha));
                        GuiDrawText(text[startIndex + i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(GuiControl::LISTVIEW, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, TEXT_COLOR_PRESSED)), guiAlpha));
                    }
                    else if ((startIndex + i) == itemFocused)
                    {
                        // Draw item focused
                        GuiDrawRectangle(itemBounds, GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BORDER_COLOR_FOCUSED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BASE_COLOR_FOCUSED)), guiAlpha));
                        GuiDrawText(text[startIndex + i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(GuiControl::LISTVIEW, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, TEXT_COLOR_FOCUSED)), guiAlpha));
                    }
                    else
                    {
                        // Draw item normal
                        GuiDrawText(text[startIndex + i], GetTextBounds(GuiControl::DEFAULT, itemBounds), GuiGetStyle(GuiControl::LISTVIEW, TEXT_ALIGNMENT), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, TEXT_COLOR_NORMAL)), guiAlpha));
                    }
                }

                // Update item raylib::Rectangle y position for next item
                itemBounds.y += (GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING));
            }

            if (useScrollBar)
            {
                raylib::Rectangle scrollBarBounds = {
                    bounds.x + bounds.width - GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_WIDTH) - GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH),
                    bounds.y + GuiGetStyle(GuiControl::LISTVIEW, GuiControlProperty::BORDER_WIDTH), (float)GuiGetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH),
                    bounds.height - 2*GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH)
                };

                // Calculate percentage of visible items and apply same percentage to scrollbar
                float percentVisible = (float)(endIndex - startIndex)/count;
                float sliderSize = bounds.height*percentVisible;

                int prevSliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);   // Save default slider size
                int prevScrollSpeed = GuiGetStyle(SCROLLBAR, SCROLL_SPEED); // Save default scroll speed
                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, (int)sliderSize);            // Change slider size
                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SPEED, count - visibleItems); // Change scroll speed

                startIndex = GuiScrollBar(scrollBarBounds, startIndex, 0, count - visibleItems);

                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SPEED, prevScrollSpeed); // Reset scroll speed to default
                GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, prevSliderSize); // Reset slider size to default
            }
            //--------------------------------------------------------------------

            if (focus != NULL) *focus = itemFocused;
            if (scrollIndex != NULL) *scrollIndex = startIndex;

            return itemSelected;
        }

        // Color Panel control
        raylib::Color RayGui::GuiColorPanel(raylib::Rectangle bounds, const char *text, raylib::Color color)
        {
            const raylib::Color colWhite = { 255, 255, 255, 255 };
            const raylib::Color colBlack = { 0, 0, 0, 255 };

            GuiState state = guiState;
            raylib::Vector2 pickerSelector = { 0 };

            raylib::Vector3 vcolor = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f };
            raylib::Vector3 hsv = ConvertRGBtoHSV(vcolor);

            pickerSelector.x = bounds.x + (float)hsv.y*bounds.width;            // HSV: Saturation
            pickerSelector.y = bounds.y + (1.0f - (float)hsv.z)*bounds.height;  // HSV: Value

            float hue = -1.0f;
            raylib::Vector3 maxHue = { hue >= 0.0f ? hue : hsv.x, 1.0f, 1.0f };
            raylib::Vector3 rgbHue = ConvertHSVtoRGB(maxHue);
            raylib::Color maxHueCol = { (unsigned char)(255.0f*rgbHue.x),
                              (unsigned char)(255.0f*rgbHue.y),
                              (unsigned char)(255.0f*rgbHue.z), 255 };

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON))
                    {
                        state = STATE_PRESSED;
                        pickerSelector = mousePoint;

                        // Calculate color from picker
                        raylib::Vector2 colorPick = { pickerSelector.x - bounds.x, pickerSelector.y - bounds.y };

                        colorPick.x /= (float)bounds.width;     // Get normalized value on x
                        colorPick.y /= (float)bounds.height;    // Get normalized value on y

                        hsv.y = colorPick.x;
                        hsv.z = 1.0f - colorPick.y;

                        raylib::Vector3 rgb = ConvertHSVtoRGB(hsv);

                        // NOTE: Vector3ToColor() only available on raylib 1.8.1
                        color = raylib::Color{ (unsigned char)(255.0f*rgb.x),
                                         (unsigned char)(255.0f*rgb.y),
                                         (unsigned char)(255.0f*rgb.z),
                                         (unsigned char)(255.0f*(float)color.a/255.0f) };

                    }
                    else state = STATE_FOCUSED;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (state != GuiState::STATE_DISABLED)
            {
                DrawRectangleGradientEx(bounds, Fade(colWhite, guiAlpha), Fade(colWhite, guiAlpha), Fade(maxHueCol, guiAlpha), Fade(maxHueCol, guiAlpha));
                DrawRectangleGradientEx(bounds, Fade(colBlack, 0), Fade(colBlack, guiAlpha), Fade(colBlack, guiAlpha), Fade(colBlack, 0));

                // Draw color picker: selector
                raylib::Rectangle selector = { pickerSelector.x - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, pickerSelector.y - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE), (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE) };
                GuiDrawRectangle(selector, 0, raylib::BLANK, Fade(colWhite, guiAlpha));
            }
            else
            {
                DrawRectangleGradientEx(bounds, Fade(Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_COLOR_DISABLED)), 0.6f), guiAlpha));
            }

            GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), guiAlpha), raylib::BLANK);
            //--------------------------------------------------------------------

            return color;
        }

        // Color Bar Alpha control
        // NOTE: Returns alpha value normalized [0..1]
        float RayGui::GuiColorBarAlpha(raylib::Rectangle bounds, const char *text, float alpha)
        {
            #if !defined(RAYGUI_COLORBARALPHA_CHECKED_SIZE)
                #define RAYGUI_COLORBARALPHA_CHECKED_SIZE   10
            #endif

            GuiState state = guiState;
            raylib::Rectangle selector = { (float)bounds.x + alpha*bounds.width - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.y - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT), (float)bounds.height + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2 };

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds) ||
                    CheckCollisionPointRec(mousePoint, selector))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON))
                    {
                        state = STATE_PRESSED;

                        alpha = (mousePoint.x - bounds.x)/bounds.width;
                        if (alpha <= 0.0f) alpha = 0.0f;
                        if (alpha >= 1.0f) alpha = 1.0f;
                        //selector.x = bounds.x + (int)(((alpha - 0)/(100 - 0))*(bounds.width - 2*GuiGetStyle(SLIDER, GuiControlProperty::BORDER_WIDTH))) - selector.width/2;
                    }
                    else state = STATE_FOCUSED;
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------

            // Draw alpha bar: checked background
            if (state != GuiState::STATE_DISABLED)
            {
                int checksX = (int)bounds.width/RAYGUI_COLORBARALPHA_CHECKED_SIZE;
                int checksY = (int)bounds.height/RAYGUI_COLORBARALPHA_CHECKED_SIZE;

                for (int x = 0; x < checksX; x++)
                {
                    for (int y = 0; y < checksY; y++)
                    {
                        raylib::Rectangle check = { bounds.x + x*RAYGUI_COLORBARALPHA_CHECKED_SIZE, bounds.y + y*RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE };
                        GuiDrawRectangle(check, 0, raylib::BLANK, ((x + y)%2)? Fade(Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_COLOR_DISABLED)), 0.4f), guiAlpha) : Fade(Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.4f), guiAlpha));
                    }
                }

                DrawRectangleGradientEx(bounds, raylib::Color{ 255, 255, 255, 0 }, raylib::Color{ 255, 255, 255, 0 }, Fade(raylib::Color{ 0, 0, 0, 255 }, guiAlpha), Fade(raylib::Color{ 0, 0, 0, 255 }, guiAlpha));
            }
            else DrawRectangleGradientEx(bounds, Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha));

            GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), guiAlpha), raylib::BLANK);

            // Draw alpha bar: selector
            GuiDrawRectangle(selector, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), guiAlpha));
            //--------------------------------------------------------------------

            return alpha;
        }

        // Color Bar Hue control
        // Returns hue value normalized [0..1]
        // NOTE: Other similar bars (for reference):
        //      Color GuiColorBarSat() [WHITE->color]
        //      Color GuiColorBarValue() [BLACK->color], HSV/HSL
        //      float GuiColorBarLuminance() [BLACK->WHITE]
        float RayGui::GuiColorBarHue(raylib::Rectangle bounds, const char *text, float hue)
        {
            GuiState state = guiState;
            raylib::Rectangle selector = { (float)bounds.x - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)bounds.y + hue/360.0f*bounds.height - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2, (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT) };

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds) ||
                    CheckCollisionPointRec(mousePoint, selector))
                {
                    if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON))
                    {
                        state = STATE_PRESSED;

                        hue = (mousePoint.y - bounds.y)*360/bounds.height;
                        if (hue <= 0.0f) hue = 0.0f;
                        if (hue >= 359.0f) hue = 359.0f;

                    }
                    else state = STATE_FOCUSED;

                    /*if (IsKeyDown(KEY_UP))
                    {
                        hue -= 2.0f;
                        if (hue <= 0.0f) hue = 0.0f;
                    }
                    else if (IsKeyDown(KEY_DOWN))
                    {
                        hue += 2.0f;
                        if (hue >= 360.0f) hue = 360.0f;
                    }*/
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            if (state != GuiState::STATE_DISABLED)
            {
                // Draw hue bar:color bars
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y), (int)bounds.width, (int)ceilf(bounds.height/6),  Fade(raylib::Color { 255, 0, 0, 255 }, guiAlpha), Fade(raylib::Color { 255, 255, 0, 255 }, guiAlpha));
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + bounds.height/6), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(raylib::Color { 255, 255, 0, 255 }, guiAlpha), Fade(raylib::Color { 0, 255, 0, 255 }, guiAlpha));
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 2*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(raylib::Color { 0, 255, 0, 255 }, guiAlpha), Fade(raylib::Color { 0, 255, 255, 255 }, guiAlpha));
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 3*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(raylib::Color { 0, 255, 255, 255 }, guiAlpha), Fade(raylib::Color { 0, 0, 255, 255 }, guiAlpha));
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 4*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(raylib::Color { 0, 0, 255, 255 }, guiAlpha), Fade(raylib::Color { 255, 0, 255, 255 }, guiAlpha));
                 raylib::DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 5*(bounds.height/6)), (int)bounds.width, (int)(bounds.height/6), Fade(raylib::Color { 255, 0, 255, 255 }, guiAlpha), Fade(raylib::Color { 255, 0, 0, 255 }, guiAlpha));
            }
            else DrawRectangleGradientV((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height, Fade(Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha));

            GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), guiAlpha), raylib::BLANK);

            // Draw hue bar: selector
            GuiDrawRectangle(selector, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), guiAlpha));
            //--------------------------------------------------------------------

            return hue;
        }

        // Color Picker control
        // NOTE: It's divided in multiple controls:
        //      Color GuiColorPanel(raylib::Rectangle bounds, Color color)
        //      float GuiColorBarAlpha(raylib::Rectangle bounds, float alpha)
        //      float GuiColorBarHue(raylib::Rectangle bounds, float value)
        // NOTE: bounds define GuiColorPanel() size
        raylib::Color RayGui::GuiColorPicker(raylib::Rectangle bounds, const char *text, raylib::Color color)
        {
            color = GuiColorPanel(bounds, NULL, color);

            raylib::Rectangle boundsHue = { (float)bounds.x + bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_PADDING), (float)bounds.y, (float)GuiGetStyle(COLORPICKER, HUEBAR_WIDTH), (float)bounds.height };
            //raylib::Rectangle boundsAlpha = { bounds.x, bounds.y + bounds.height + GuiGetStyle(COLORPICKER, BARS_PADDING), bounds.width, GuiGetStyle(COLORPICKER, BARS_THICK) };

            raylib::Vector3 hsv = ConvertRGBtoHSV(raylib::Vector3{ color.r/255.0f, color.g/255.0f, color.b/255.0f });
            hsv.x = GuiColorBarHue(boundsHue, NULL, hsv.x);
            //color.a = (unsigned char)(GuiColorBarAlpha(boundsAlpha, (float)color.a/255.0f)*255.0f);
            raylib::Vector3 rgb = ConvertHSVtoRGB(hsv);

            color = raylib::Color{ (unsigned char)roundf(rgb.x*255.0f), (unsigned char)roundf(rgb.y*255.0f), (unsigned char)roundf(rgb.z*255.0f), color.a };

            return color;
        }

        // Message Box control
        int RayGui::GuiMessageBox(raylib::Rectangle bounds, const char *title, const char *message, const char *buttons)
        {
            #if !defined(RAYGUI_MESSAGEBOX_BUTTON_HEIGHT)
                #define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT    24
            #endif
            #if !defined(RAYGUI_MESSAGEBOX_BUTTON_PADDING)
                #define RAYGUI_MESSAGEBOX_BUTTON_PADDING   12
            #endif

            int clicked = -1;    // Returns clicked button from buttons list, 0 refers to closed window button

            int buttonCount = 0;
            const char **buttonsText = GuiTextSplit(buttons, &buttonCount, NULL);
            raylib::Rectangle buttonBounds = { 0 };
            buttonBounds.x = bounds.x + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
            buttonBounds.y = bounds.y + bounds.height - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT - RAYGUI_MESSAGEBOX_BUTTON_PADDING;
            buttonBounds.width = (bounds.width - RAYGUI_MESSAGEBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
            buttonBounds.height = RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

            raylib::Vector2 textSize = MeasureTextEx(guiFont, message, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), 1);

            raylib::Rectangle textBounds = { 0 };
            textBounds.x = bounds.x + bounds.width/2 - textSize.x/2;
            textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
            textBounds.width = textSize.x;
            textBounds.height = bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 3*RAYGUI_MESSAGEBOX_BUTTON_PADDING - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

            // Draw control
            //--------------------------------------------------------------------
            if (GuiWindowBox(bounds, title)) clicked = 0;

            int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            GuiLabel(textBounds, message);
            GuiSetStyle(GuiControl::LABEL, TEXT_ALIGNMENT, prevTextAlignment);

            prevTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

            for (int i = 0; i < buttonCount; i++)
            {
                if (GuiButton(buttonBounds, buttonsText[i])) clicked = i + 1;
                buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
            }

            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, prevTextAlignment);
            //--------------------------------------------------------------------

            return clicked;
        }

        // Text Input Box control, ask for text
        int RayGui::GuiTextInputBox(raylib::Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, int *secretViewActive)
        {
            #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT)
                #define RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT      28
            #endif
            #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_PADDING)
                #define RAYGUI_TEXTINPUTBOX_BUTTON_PADDING     12
            #endif
            #if !defined(RAYGUI_TEXTINPUTBOX_HEIGHT)
                #define RAYGUI_TEXTINPUTBOX_HEIGHT             28
            #endif

            // Used to enable text edit mode
            // WARNING: No more than one GuiTextInputBox() should be open at the same time
            static bool textEditMode = false;

            int btnIndex = -1;

            int buttonCount = 0;
            const char **buttonsText = GuiTextSplit(buttons, &buttonCount, NULL);
            raylib::Rectangle buttonBounds = { 0 };
            buttonBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
            buttonBounds.y = bounds.y + bounds.height - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
            buttonBounds.width = (bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
            buttonBounds.height = RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT;

            int messageInputHeight = (int)bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - GuiGetStyle(STATUSBAR, GuiControlProperty::BORDER_WIDTH) - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - 2*RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;

            raylib::Rectangle textBounds = { 0 };
            if (message != NULL)
            {
                raylib::Vector2 textSize = MeasureTextEx(guiFont, message, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), 1);

                textBounds.x = bounds.x + bounds.width/2 - textSize.x/2;
                textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + messageInputHeight/4 - textSize.y/2;
                textBounds.width = textSize.x;
                textBounds.height = textSize.y;
            }

            raylib::Rectangle textBoxBounds = { 0 };
            textBoxBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
            textBoxBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_TEXTINPUTBOX_HEIGHT/2;
            if (message == NULL) textBoxBounds.y = bounds.y + 24 + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
            else textBoxBounds.y += (messageInputHeight/2 + messageInputHeight/4);
            textBoxBounds.width = bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*2;
            textBoxBounds.height = RAYGUI_TEXTINPUTBOX_HEIGHT;

            // Draw control
            //--------------------------------------------------------------------
            if (GuiWindowBox(bounds, title)) btnIndex = 0;

            // Draw message if available
            if (message != NULL)
            {
                int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
                GuiSetStyle(GuiControl::LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiLabel(textBounds, message);
                GuiSetStyle(GuiControl::LABEL, TEXT_ALIGNMENT, prevTextAlignment);
            }

            if (secretViewActive != NULL)
            {
                static char stars[] = "****************";
                if (GuiTextBox(raylib::Rectangle{ textBoxBounds.x, textBoxBounds.y, textBoxBounds.width - 4 - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.height }, 
                    ((*secretViewActive == 1) || textEditMode)? text : stars, textMaxSize, textEditMode)) textEditMode = !textEditMode;

                *secretViewActive = GuiToggle(raylib::Rectangle{ textBoxBounds.x + textBoxBounds.width - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.y, RAYGUI_TEXTINPUTBOX_HEIGHT, RAYGUI_TEXTINPUTBOX_HEIGHT }, (*secretViewActive == 1)? "#44#" : "#45#", *secretViewActive);
            }
            else
            {
                if (GuiTextBox(textBoxBounds, text, textMaxSize, textEditMode)) textEditMode = !textEditMode;
            }

            int prevBtnTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

            for (int i = 0; i < buttonCount; i++)
            {
                if (GuiButton(buttonBounds, buttonsText[i])) btnIndex = i + 1;
                buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
            }

            GuiSetStyle(GuiControl::BUTTON, TEXT_ALIGNMENT, prevBtnTextAlignment);
            //--------------------------------------------------------------------

            return btnIndex;
        }

        // Grid control
        // NOTE: Returns grid mouse-hover selected cell
        // About drawing lines at subpixel spacing, simple put, not easy solution:
        // https://stackoverflow.com/questions/4435450/2d-opengl-drawing-lines-that-dont-exactly-fit-pixel-raster
        raylib::Vector2 RayGui::GuiGrid(raylib::Rectangle bounds, const char *text, float spacing, int subdivs)
        {
            // Grid lines alpha amount
            #if !defined(RAYGUI_GRID_ALPHA)
                #define RAYGUI_GRID_ALPHA    0.15f
            #endif

            GuiState state = guiState;
            raylib::Vector2 mousePoint = raylib::GetMousePosition();
            raylib::Vector2 currentCell = { -1, -1 };

            int linesV = ((int)(bounds.width/spacing))*subdivs + 1;
            int linesH = ((int)(bounds.height/spacing))*subdivs + 1;

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    // NOTE: Cell values must be rounded to int
                    currentCell.x = (int)((mousePoint.x - bounds.x)/spacing);
                    currentCell.y = (int)((mousePoint.y - bounds.y)/spacing);
                }
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------

            // TODO: Draw background panel?

            switch (state)
            {
                case STATE_NORMAL:
                {
                    if (subdivs > 0)
                    {
                        // Draw vertical grid lines
                        for (int i = 0; i < linesV; i++)
                        {
                            raylib::Rectangle lineV = { bounds.x + spacing*i/subdivs, bounds.y, 1, bounds.height };
                            GuiDrawRectangle(lineV, 0, raylib::BLANK, ((i%subdivs) == 0) ? Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR)), RAYGUI_GRID_ALPHA*4) : Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR)), RAYGUI_GRID_ALPHA));
                        }

                        // Draw horizontal grid lines
                        for (int i = 0; i < linesH; i++)
                        {
                            raylib::Rectangle lineH = { bounds.x, bounds.y + spacing*i/subdivs, bounds.width, 1 };
                            GuiDrawRectangle(lineH, 0, raylib::BLANK, ((i%subdivs) == 0) ? Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR)), RAYGUI_GRID_ALPHA*4) : Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR)), RAYGUI_GRID_ALPHA));
                        }
                    }
                } break;
                default: break;
            }

            return currentCell;
        }

        //----------------------------------------------------------------------------------
        // Styles loading functions
        //----------------------------------------------------------------------------------

        // Load raygui style file (.rgs)
        // NOTE: By default a binary file is expected, that file could contain a custom font,
        // in that case, custom font image atlas is GRAY+ALPHA and pixel data can be compressed (DEFLATE)
        void RayGui::GuiLoadStyle(const char *fileName)
        {
            /*
            #define MAX_LINE_BUFFER_SIZE    256

            bool tryBinary = false;

            // Try reading the files as text file first
            FILE *rgsFile = fopen(fileName, "rt");

            if (rgsFile != NULL)
            {
                char buffer[MAX_LINE_BUFFER_SIZE] = { 0 };
                fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);

                if (buffer[0] == '#')
                {
                    int controlId = 0;
                    int propertyId = 0;
                    unsigned int propertyValue = 0;

                    while (!feof(rgsFile))
                    {
                        switch (buffer[0])
                        {
                            case 'p':
                            {
                                // Style property: p <control_id> <property_id> <property_value> <property_name>

                                sscanf(buffer, "p %d %d 0x%x", &controlId, &propertyId, &propertyValue);
                                GuiSetStyle( controlId, propertyId, (int)propertyValue);

                            } break;
                            case 'f':
                            {
                                // Style font: f <gen_font_size> <charmap_file> <font_file>

                                int fontSize = 0;
                                char charmapFileName[256] = { 0 };
                                char fontFileName[256] = { 0 };
                                sscanf(buffer, "f %d %s %[^\r\n]s", &fontSize, charmapFileName, fontFileName);

                                raylib::Font font = { 0 };

                                if (charmapFileName[0] != '0')
                                {
                                    // Load characters from charmap file,
                                    // expected '\n' separated list of integer values
                                    char *charValues = raylib::LoadFileText(charmapFileName);
                                    if (charValues != NULL)
                                    {
                                        int glyphCount = 0;
                                        const char **chars = raylib::TextSplit(charValues, '\n', &glyphCount);

                                        int *values = (int *)malloc(glyphCount*sizeof(int));
                                        for (int i = 0; i < glyphCount; i++) values[i] = raylib::TextToInteger(chars[i]);

                                        if (font.texture.id != raylib::GetFontDefault().texture.id) raylib::UnloadTexture(font.texture);
                                        font = raylib::LoadFontEx(raylib::TextFormat("%s/%s", raylib::GetDirectoryPath(fileName), fontFileName), fontSize, values, glyphCount);
                                        if (font.texture.id == 0) font = raylib::GetFontDefault();

                                        free(values);
                                    }
                                }
                                else
                                {
                                    if (font.texture.id != raylib::GetFontDefault().texture.id) raylib::UnloadTexture(font.texture);
                                    font = raylib::LoadFontEx(raylib::TextFormat("%s/%s", raylib::GetDirectoryPath(fileName), fontFileName), fontSize, NULL, 0);
                                    if (font.texture.id == 0) font = raylib::GetFontDefault();
                                }

                                if ((font.texture.id > 0) && (font.glyphCount > 0)) GuiSetFont(font);

                            } break;
                            default: break;
                        }

                        fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);
                    }
                }
                else tryBinary = true;

                fclose(rgsFile);
            }

            if (tryBinary)
            {
                rgsFile = fopen(fileName, "rb");

                if (rgsFile == NULL) return;

                char signature[5] = { 0 };
                short version = 0;
                short reserved = 0;
                int propertyCount = 0;

                fread(signature, 1, 4, rgsFile);
                fread(&version, 1, sizeof(short), rgsFile);
                fread(&reserved, 1, sizeof(short), rgsFile);
                fread(&propertyCount, 1, sizeof(int), rgsFile);

                if ((signature[0] == 'r') &&
                    (signature[1] == 'G') &&
                    (signature[2] == 'S') &&
                    (signature[3] == ' '))
                {
                    short controlId = 0;
                    short propertyId = 0;
                    unsigned int propertyValue = 0;

                    for (int i = 0; i < propertyCount; i++)
                    {
                        fread(&controlId, 1, sizeof(short), rgsFile);
                        fread(&propertyId, 1, sizeof(short), rgsFile);
                        fread(&propertyValue, 1, sizeof(unsigned int), rgsFile);

                        if (controlId == 0) // GuiControl::DEFAULT control
                        {
                            // If a GuiControl::DEFAULT property is loaded, it is propagated to all controls
                            // NOTE: All GuiControl::DEFAULT properties should be defined first in the file
                            GuiSetStyle(DEFAULT, (int)propertyId, propertyValue);

                            if (propertyId < RAYGUI_MAX_PROPS_BASE) for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) GuiSetStyle(i, (int)propertyId, propertyValue);
                        }
                        else GuiSetStyle((int)controlId, (int)propertyId, propertyValue);
                    }

                    // Font loading is highly dependant on raylib API to load font data and image
        #if !defined(RAYGUI_STANDALONE)
                    // Load custom font if available
                    int fontDataSize = 0;
                    fread(&fontDataSize, 1, sizeof(int), rgsFile);

                    if (fontDataSize > 0)
                    {
                        raylib::Font font = { 0 };
                        int fontType = 0;   // 0-Normal, 1-SDF
                        raylib::Rectangle whiteRec = { 0 };

                        fread(&font.baseSize, 1, sizeof(int), rgsFile);
                        fread(&font.glyphCount, 1, sizeof(int), rgsFile);
                        fread(&fontType, 1, sizeof(int), rgsFile);

                        // Load font white raylib::Rectangle
                        fread(&whiteRec, 1, sizeof(raylib::Rectangle, rgsFile);

                        // Load font image parameters
                        int fontImageUncompSize = 0;
                        int fontImageCompSize = 0;
                        fread(&fontImageUncompSize, 1, sizeof(int), rgsFile);
                        fread(&fontImageCompSize, 1, sizeof(int), rgsFile);

                        raylib::Image imFont = { 0 };
                        imFont.mipmaps = 1;
                        fread(&imFont.width, 1, sizeof(int), rgsFile);
                        fread(&imFont.height, 1, sizeof(int), rgsFile);
                        fread(&imFont.format, 1, sizeof(int), rgsFile);

                        if (fontImageCompSize < fontImageUncompSize)
                        {
                            // Compressed font atlas image data (DEFLATE), it requires DecompressData()
                            int dataUncompSize = 0;
                            unsigned char *compData = (unsigned char *)malloc(fontImageCompSize);
                            fread(compData, 1, fontImageCompSize, rgsFile);
                            imFont.data = raylib::DecompressData(compData, fontImageCompSize, &dataUncompSize);

                            // Security check, dataUncompSize must match the provided fontImageUncompSize
                            if (dataUncompSize != fontImageUncompSize) RAYGUI_LOG("WARNING: Uncompressed font atlas image data could be corrupted");

                            free(compData);
                        }
                        else
                        {
                            // Font atlas image data is not compressed
                            imFont.data = (unsigned char *)malloc(fontImageUncompSize);
                            fread(imFont.data, 1, fontImageUncompSize, rgsFile);
                        }

                        if (font.texture.id != raylib::GetFontDefault().texture.id) raylib::UnloadTexture(font.texture);
                        font.texture = LoadTextureFromImage(imFont);
                        if (font.texture.id == 0) font = raylib::GetFontDefault();

                        free(imFont.data);

                        // Load font recs data
                        font.recs = (raylib::Rectangle *)calloc(font.glyphCount, sizeof(raylib::Rectangle);
                        for (int i = 0; i < font.glyphCount; i++) fread(&font.recs[i], 1, sizeof(raylib::Rectangle, rgsFile);

                        // Load font chars info data
                        font.glyphs = (raylib::GlyphInfo *)calloc(font.glyphCount, sizeof(raylib::GlyphInfo));
                        for (int i = 0; i < font.glyphCount; i++)
                        {
                            fread(&font.glyphs[i].value, 1, sizeof(int), rgsFile);
                            fread(&font.glyphs[i].offsetX, 1, sizeof(int), rgsFile);
                            fread(&font.glyphs[i].offsetY, 1, sizeof(int), rgsFile);
                            fread(&font.glyphs[i].advanceX, 1, sizeof(int), rgsFile);
                        }

                        GuiSetFont(font);

                        // Set font texture source raylib::Rectangle to be used as white texture to draw shapes
                        // NOTE: This way, all gui can be draw using a single draw call
                        if ((whiteRec.width != 0) && (whiteRec.height != 0)) SetShapesTexture(font.texture, whiteRec);
                    }
        #endif
                }

                fclose(rgsFile);
            }

            */

        }

        // Load style default over global style
        void RayGui::GuiLoadStyleDefault(void)
        {
            // We set this variable first to avoid cyclic function calls
            // when calling GuiSetStyle(GuiControl::) and GuiGetStyle()
            guiStyleLoaded = true;

            // Initialize default LIGHT style property values
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_NORMAL, 0x838383ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_NORMAL, 0xc9c9c9ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_NORMAL, 0x686868ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_FOCUSED, 0x5bb2d9ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_FOCUSED, 0xc9effeff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_FOCUSED, 0x6c9bbcff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_PRESSED, 0x0492c7ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_PRESSED, 0x97e8ffff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_PRESSED, 0x368bafff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_DISABLED, 0xb5c1c2ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_DISABLED, 0xe6e9e9ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_DISABLED, 0xaeb7b8ff);
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_WIDTH, 1);                       // WARNING: Some controls use other values
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_PADDING, 0);                       // WARNING: Some controls use other values
            GuiSetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_CENTER); // WARNING: Some controls use other values

            // Initialize control-specific property values
            // NOTE: Those properties are in default list but require specific values by control type
            GuiSetStyle(GuiControl::LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_WIDTH, 2);
            GuiSetStyle(GuiControl::SLIDER, TEXT_PADDING, 4);
            GuiSetStyle(GuiControl::CHECKBOX, TEXT_PADDING, 4);
            GuiSetStyle(GuiControl::CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
            GuiSetStyle(GuiControl::TEXTBOX, TEXT_PADDING, 4);
            GuiSetStyle(GuiControl::TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(GuiControl::VALUEBOX, TEXT_PADDING, 4);
            GuiSetStyle(GuiControl::VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(GuiControl::SPINNER, TEXT_PADDING, 4);
            GuiSetStyle(GuiControl::SPINNER, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(GuiControl::STATUSBAR, TEXT_PADDING, 8);
            GuiSetStyle(GuiControl::STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

            // Initialize extended property values
            // NOTE: By default, extended property values are initialized to 0
            GuiSetStyle(GuiControl::DEFAULT, TEXT_SIZE, 10);                // GuiControl::DEFAULT, shared by all controls
            GuiSetStyle(GuiControl::DEFAULT, TEXT_SPACING, 1);              // GuiControl::DEFAULT, shared by all controls
            GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR, 0x90abb5ff);       // GuiControl::DEFAULT specific property
            GuiSetStyle(GuiControl::DEFAULT, BACKGROUND_COLOR, 0xf5f5f5ff); // GuiControl::DEFAULT specific property
            GuiSetStyle(GuiControl::TOGGLE, GROUP_PADDING, 2);
            GuiSetStyle(GuiControl::SLIDER, SLIDER_WIDTH, 16);
            GuiSetStyle(GuiControl::SLIDER, SLIDER_PADDING, 1);
            GuiSetStyle(GuiControl::PROGRESSBAR, PROGRESS_PADDING, 1);
            GuiSetStyle(GuiControl::CHECKBOX, CHECK_PADDING, 1);
            GuiSetStyle(GuiControl::COMBOBOX, COMBO_BUTTON_WIDTH, 32);
            GuiSetStyle(GuiControl::COMBOBOX, COMBO_BUTTON_SPACING, 2);
            GuiSetStyle(GuiControl::DROPDOWNBOX, ARROW_PADDING, 16);
            GuiSetStyle(GuiControl::DROPDOWNBOX, DROPDOWN_ITEMS_SPACING, 2);
            GuiSetStyle(GuiControl::TEXTBOX, TEXT_LINES_SPACING, 4);
            GuiSetStyle(GuiControl::TEXTBOX, TEXT_INNER_PADDING, 4);
            GuiSetStyle(GuiControl::SPINNER, SPIN_BUTTON_WIDTH, 24);
            GuiSetStyle(GuiControl::SPINNER, SPIN_BUTTON_SPACING, 2);
            GuiSetStyle(GuiControl::SCROLLBAR, GuiControlProperty::BORDER_WIDTH, 0);
            GuiSetStyle(GuiControl::SCROLLBAR, ARROWS_VISIBLE, 0);
            GuiSetStyle(GuiControl::SCROLLBAR, ARROWS_SIZE, 6);
            GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_PADDING, 0);
            GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SLIDER_SIZE, 16);
            GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_PADDING, 0);
            GuiSetStyle(GuiControl::SCROLLBAR, SCROLL_SPEED, 12);
            GuiSetStyle(GuiControl::LISTVIEW, LIST_ITEMS_HEIGHT, 24);
            GuiSetStyle(GuiControl::LISTVIEW, LIST_ITEMS_SPACING, 2);
            GuiSetStyle(GuiControl::LISTVIEW, SCROLLBAR_WIDTH, 12);
            GuiSetStyle(GuiControl::LISTVIEW, SCROLLBAR_SIDE, SCROLLBAR_RIGHT_SIDE);
            GuiSetStyle(GuiControl::COLORPICKER, COLOR_SELECTOR_SIZE, 8);
            GuiSetStyle(GuiControl::COLORPICKER, HUEBAR_WIDTH, 16);
            GuiSetStyle(GuiControl::COLORPICKER, HUEBAR_PADDING, 8);
            GuiSetStyle(GuiControl::COLORPICKER, HUEBAR_SELECTOR_HEIGHT, 8);
            GuiSetStyle(GuiControl::COLORPICKER, HUEBAR_SELECTOR_OVERFLOW, 2);

            guiFont = raylib::GetFontDefault();     // Initialize default font
        }

        // Get text with icon id prepended
        // NOTE: Useful to add icons by name id (enum) instead of
        // a number that can change between ricon versions
        const char *GuiIconText(int iconId, const char *text)
        {
        #if defined(RAYGUI_NO_ICONS)
            return NULL;
        #else
            static char buffer[1024] = { 0 };
            static char iconBuffer[6] = { 0 };

            if (text != NULL)
            {
                memset(buffer, 0, 1024);
                sprintf(buffer, "#%03i#", iconId);
        
                for (int i = 5; i < 1024; i++)
                {
                    buffer[i] = text[i - 5];
                    if (text[i - 5] == '\0') break;
                }
    
                return buffer;
            }
            else 
            {
                sprintf(iconBuffer, "#%03i#", iconId & 0x1ff);
        
                return iconBuffer;
            }
        #endif
        }

        #if !defined(RAYGUI_NO_ICONS)

        // Get full icons data pointer
        unsigned int *GuiGetIcons(void) { return guiIcons; }

        // Load raygui icons file (.rgi)
        // NOTE: In case nameIds are required, they can be requested with loadIconsName,
        // they are returned as a guiIconsName[iconCount][RAYGUI_ICON_MAX_NAME_LENGTH],
        // WARNING: guiIconsName[]][] memory should be manually freed!
        char **GuiLoadIcons(const char *fileName, bool loadIconsName)
        {
            // Style File Structure (.rgi)
            // ------------------------------------------------------
            // Offset  | Size    | Type       | Description
            // ------------------------------------------------------
            // 0       | 4       | char       | Signature: "rGI "
            // 4       | 2       | short      | Version: 100
            // 6       | 2       | short      | reserved

            // 8       | 2       | short      | Num icons (N)
            // 10      | 2       | short      | Icons size (Options: 16, 32, 64) (S)

            // Icons name id (32 bytes per name id)
            // foreach (icon)
            // {
            //   12+32*i  | 32   | char       | Icon NameId
            // }

            // Icons data: One bit per pixel, stored as unsigned int array (depends on icon size)
            // S*S pixels/32bit per unsigned int = K unsigned int per icon
            // foreach (icon)
            // {
            //   ...   | K       | unsigned int | Icon Data
            // }

            FILE *rgiFile = fopen(fileName, "rb");

            char **guiIconsName = NULL;

            if (rgiFile != NULL)
            {
                char signature[5] = { 0 };
                short version = 0;
                short reserved = 0;
                short iconCount = 0;
                short iconSize = 0;

                fread(signature, 1, 4, rgiFile);
                fread(&version, 1, sizeof(short), rgiFile);
                fread(&reserved, 1, sizeof(short), rgiFile);
                fread(&iconCount, 1, sizeof(short), rgiFile);
                fread(&iconSize, 1, sizeof(short), rgiFile);

                if ((signature[0] == 'r') &&
                    (signature[1] == 'G') &&
                    (signature[2] == 'I') &&
                    (signature[3] == ' '))
                {
                    if (loadIconsName)
                    {
                        guiIconsName = (char **)malloc(iconCount*sizeof(char **));
                        for (int i = 0; i < iconCount; i++)
                        {
                            guiIconsName[i] = (char *)malloc(RAYGUI_ICON_MAX_NAME_LENGTH);
                            fread(guiIconsName[i], RAYGUI_ICON_MAX_NAME_LENGTH, 1, rgiFile);
                        }
                    }
                    else fseek(rgiFile, iconCount*RAYGUI_ICON_MAX_NAME_LENGTH, SEEK_CUR);

                    // Read icons data directly over guiIcons data array
                    fread(guiIcons, iconCount*(iconSize*iconSize/32), sizeof(unsigned int), rgiFile);
                }

                fclose(rgiFile);
            }

            return guiIconsName;
        }

        // Draw selected icon using rectangles pixel-by-pixel
        void RayGui::GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color)
        {
            #define BIT_CHECK(a,b) ((a) & (1u<<(b)))

            for (int i = 0, y = 0; i < RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32; i++)
            {
                for (int k = 0; k < 32; k++)
                {
                    if (BIT_CHECK(guiIcons[iconId*RAYGUI_ICON_DATA_ELEMENTS + i], k))
                    {
                    #if !defined(RAYGUI_STANDALONE)
                        DrawRectangle(posX + (k%RAYGUI_ICON_SIZE)*pixelSize, posY + y*pixelSize, pixelSize, pixelSize, color);
                    #endif
                    }

                    if ((k == 15) || (k == 31)) y++;
                }
            }
        }

        // Get icon bit data
        // NOTE: Bit data array grouped as unsigned int (RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32 elements)
        unsigned int* RayGui::GuiGetIconData(int iconId)
        {
            static unsigned int iconData[RAYGUI_ICON_DATA_ELEMENTS] = { 0 };
            memset(iconData, 0, RAYGUI_ICON_DATA_ELEMENTS*sizeof(unsigned int));

            if (iconId < RAYGUI_ICON_MAX_ICONS) memcpy(iconData, &guiIcons[iconId*RAYGUI_ICON_DATA_ELEMENTS], RAYGUI_ICON_DATA_ELEMENTS*sizeof(unsigned int));

            return iconData;
        }

        // Set icon bit data
        // NOTE: Data must be provided as unsigned int array (RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32 elements)
        void RayGui::GuiSetIconData(int iconId, unsigned int *data)
        {
            if (iconId < RAYGUI_ICON_MAX_ICONS) memcpy(&guiIcons[iconId*RAYGUI_ICON_DATA_ELEMENTS], data, RAYGUI_ICON_DATA_ELEMENTS*sizeof(unsigned int));
        }

        // Set icon scale (1 by default)
        void RayGui::GuiSetIconScale(unsigned int scale)
        {
            guiIconScale = (scale < 1)? 1 : scale;
        }

        // Set icon pixel value
        void RayGui::GuiSetIconPixel(int iconId, int x, int y)
        {
            #define BIT_SET(a,b)   ((a) |= (1u<<(b)))

            // This logic works for any RAYGUI_ICON_SIZE pixels icons,
            // For example, in case of 16x16 pixels, every 2 lines fit in one unsigned int data element
            BIT_SET(guiIcons[iconId*RAYGUI_ICON_DATA_ELEMENTS + y/(sizeof(unsigned int)*8/RAYGUI_ICON_SIZE)], x + (y%(sizeof(unsigned int)*8/RAYGUI_ICON_SIZE)*RAYGUI_ICON_SIZE));
        }

        // Clear icon pixel value
        void RayGui::GuiClearIconPixel(int iconId, int x, int y)
        {
            #define BIT_CLEAR(a,b) ((a) &= ~((1u)<<(b)))

            // This logic works for any RAYGUI_ICON_SIZE pixels icons,
            // For example, in case of 16x16 pixels, every 2 lines fit in one unsigned int data element
            BIT_CLEAR(guiIcons[iconId*RAYGUI_ICON_DATA_ELEMENTS + y/(sizeof(unsigned int)*8/RAYGUI_ICON_SIZE)], x + (y%(sizeof(unsigned int)*8/RAYGUI_ICON_SIZE)*RAYGUI_ICON_SIZE));
        }

        // Check icon pixel value
        bool RayGui::GuiCheckIconPixel(int iconId, int x, int y)
        {
            #define BIT_CHECK(a,b) ((a) & (1u<<(b)))

            return (BIT_CHECK(guiIcons[iconId*8 + y/2], x + (y%2*16)));
        }
        #endif      // !RAYGUI_NO_ICONS

        //----------------------------------------------------------------------------------
        // Module specific Functions Definition
        //----------------------------------------------------------------------------------
        // Gui get text width considering icon
        int RayGui::GetTextWidth(const char *text)
        {
            #if !defined(ICON_TEXT_PADDING)
                #define ICON_TEXT_PADDING   4
            #endif

            raylib::Vector2 size = { 0 };
            int textIconOffset = 0;

            if ((text != NULL) && (text[0] != '\0'))
            {
                if (text[0] == '#')
                {
                    for (int i = 1; (text[i] != '\0') && (i < 5); i++)
                    {
                        if (text[i] == '#')
                        {
                            textIconOffset = i;
                            break;
                        }
                    }
                }
        
                // Make sure guiFont is set, GuiGetStyle() initializes it lazynessly
                float fontSize = (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);
        
                size = MeasureTextEx(guiFont, text + textIconOffset, fontSize, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING));
                if (textIconOffset > 0) size.x += (RAYGUI_ICON_SIZE - ICON_TEXT_PADDING);
            }

            return (int)size.x;
        }

        // Get text bounds considering control bounds
        raylib::Rectangle RayGui::GetTextBounds(int control, raylib::Rectangle bounds)
        {
            raylib::Rectangle textBounds = bounds;

            textBounds.x = bounds.x + GuiGetStyle(control, GuiControlProperty::BORDER_WIDTH);
            textBounds.y = bounds.y + GuiGetStyle(control, GuiControlProperty::BORDER_WIDTH);
            textBounds.width = bounds.width - 2*GuiGetStyle(control, GuiControlProperty::BORDER_WIDTH);
            textBounds.height = bounds.height - 2*GuiGetStyle(control, GuiControlProperty::BORDER_WIDTH);

            // Consider TEXT_PADDING properly, depends on control type and TEXT_ALIGNMENT
            switch (control)
            {
                case COMBOBOX: bounds.width -= (GuiGetStyle(control, COMBO_BUTTON_WIDTH) + GuiGetStyle(control, COMBO_BUTTON_SPACING)); break;
                case VALUEBOX: break;   // NOTE: ValueBox text value always centered, text padding applies to label
                default:
                {
                    if (GuiGetStyle(control, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) textBounds.x -= GuiGetStyle(control, TEXT_PADDING);
                    else textBounds.x += GuiGetStyle(control, TEXT_PADDING);
                } break;
            }

            // TODO: Special cases (no label): COMBOBOX, DROPDOWNBOX, GuiControl::LISTVIEW (scrollbar?)
            // More special cases (label on side): CHECKBOX, SLIDER, VALUEBOX, SPINNER

            return textBounds;
        }

        // Get text icon if provided and move text cursor
        // NOTE: We support up to 999 values for iconId
        const char * RayGui::GetTextIcon(const char *text, int *iconId)
        {
        #if !defined(RAYGUI_NO_ICONS)
            *iconId = -1;
            if (text[0] == '#')     // Maybe we have an icon!
            {
                char iconValue[4] = { 0 };  // Maximum length for icon value: 3 digits + '\0'

                int pos = 1;
                while ((pos < 4) && (text[pos] >= '0') && (text[pos] <= '9'))
                {
                    iconValue[pos - 1] = text[pos];
                    pos++;
                }

                if (text[pos] == '#')
                {
                    *iconId = TextToInteger(iconValue);

                    // Move text pointer after icon
                    // WARNING: If only icon provided, it could point to EOL character: '\0'
                    if (*iconId >= 0) text += (pos + 1);
                }
            }
        #endif

            return text;
        }

        // Gui draw text using default font
        void RayGui::GuiDrawText(const char *text, raylib::Rectangle bounds, int alignment, raylib::Color tint)
        {
            #define TEXT_VALIGN_PIXEL_OFFSET(h)  ((int)h%2)     // Vertical alignment for pixel perfect

            #if !defined(ICON_TEXT_PADDING)
                #define ICON_TEXT_PADDING   4
            #endif

            if ((text != NULL) && (text[0] != '\0'))
            {
                int iconId = 0;
                text = GetTextIcon(text, &iconId);              // Check text for icon and move cursor

                // Get text position depending on alignment and iconId
                //---------------------------------------------------------------------------------
                raylib::Vector2 position = { bounds.x, bounds.y };

                // NOTE: We get text size after icon has been processed
                // TODO: REVIEW: We consider text size in case of line breaks! -> MeasureTextEx() depends on raylib!
                raylib::Vector2 textSize = MeasureTextEx(GuiGetFont(), text, GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING));
                //int textWidth = GetTextWidth(text);
                //int textHeight = GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE);

                // If text requires an icon, add size to measure
                if (iconId >= 0)
                {
                    textSize.x += RAYGUI_ICON_SIZE*guiIconScale;

                    // WARNING: If only icon provided, text could be pointing to EOF character: '\0'
                    if ((text != NULL) && (text[0] != '\0')) textSize.x += ICON_TEXT_PADDING;
                }

                // Check guiTextAlign global variables
                switch (alignment)
                {
                    case TEXT_ALIGN_LEFT:
                    {
                        position.x = bounds.x;
                        position.y = bounds.y + bounds.height/2 - textSize.y/2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
                    } break;
                    case TEXT_ALIGN_CENTER:
                    {
                        position.x = bounds.x + bounds.width/2 - textSize.x/2;
                        position.y = bounds.y + bounds.height/2 - textSize.y/2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
                    } break;
                    case TEXT_ALIGN_RIGHT:
                    {
                        position.x = bounds.x + bounds.width - textSize.x;
                        position.y = bounds.y + bounds.height/2 - textSize.y/2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
                    } break;
                    default: break;
                }

                // NOTE: Make sure we get pixel-perfect coordinates,
                // In case of decimals we got weird text positioning
                position.x = (float)((int)position.x);
                position.y = (float)((int)position.y);
                //---------------------------------------------------------------------------------

                // Draw text (with icon if available)
                //---------------------------------------------------------------------------------
        #if !defined(RAYGUI_NO_ICONS)
                if (iconId >= 0)
                {
                    // NOTE: We consider icon height, probably different than text size
                    GuiDrawIcon(iconId, (int)position.x, (int)(bounds.y + bounds.height/2 - RAYGUI_ICON_SIZE*guiIconScale/2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height)), guiIconScale, tint);
                    position.x += (RAYGUI_ICON_SIZE*guiIconScale + ICON_TEXT_PADDING);
                }
        #endif
                DrawTextEx(guiFont, text, position, (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SIZE), (float)GuiGetStyle(GuiControl::DEFAULT, TEXT_SPACING), tint);
                //---------------------------------------------------------------------------------
            }
        }

        // Gui draw raylib::Rectangle using default raygui plain style with borders
        void RayGui::GuiDrawRectangle(raylib::Rectangle rec, int borderWidth, raylib::Color borderColor, raylib::Color color)
        {
            if (color.a > 0)
            {
                // Draw raylib::Rectangle filled with color
                DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, color);
            }

            if (borderWidth > 0)
            {
                // Draw raylib::Rectangle border lines with color
                DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, borderWidth, borderColor);
                DrawRectangle((int)rec.x, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, borderColor);
                DrawRectangle((int)rec.x + (int)rec.width - borderWidth, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, borderColor);
                DrawRectangle((int)rec.x, (int)rec.y + (int)rec.height - borderWidth, (int)rec.width, borderWidth, borderColor);
            }
        }

        // Split controls text into multiple strings
        // Also check for multiple columns (required by GuiToggleGroup())
        const char ** RayGui::GuiTextSplit(const char *text, int *count, int *textRow)
        {
            // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
            // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
            // all used memory is static... it has some limitations:
            //      1. Maximum number of possible split strings is set by RAYGUI_TEXTSPLIT_MAX_ITEMS
            //      2. Maximum size of text to split is RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE
            // NOTE: Those definitions could be externally provided if required

            #if !defined(RAYGUI_TEXTSPLIT_MAX_ITEMS)
                #define RAYGUI_TEXTSPLIT_MAX_ITEMS        128
            #endif
            #if !defined(RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE)
                #define RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE      1024
            #endif

            static const char *result[RAYGUI_TEXTSPLIT_MAX_ITEMS] = { NULL };
            static char buffer[RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE] = { 0 };
            memset(buffer, 0, RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE);

            result[0] = buffer;
            int counter = 1;

            if (textRow != NULL) textRow[0] = 0;

            // Count how many substrings we have on text and point to every one
            for (int i = 0; i < RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE; i++)
            {
                buffer[i] = text[i];
                if (buffer[i] == '\0') break;
                else if ((buffer[i] == ';') || (buffer[i] == '\n'))
                {
                    result[counter] = buffer + i + 1;

                    if (textRow != NULL)
                    {
                        if (buffer[i] == '\n') textRow[counter] = textRow[counter - 1] + 1;
                        else textRow[counter] = textRow[counter - 1];
                    }

                    buffer[i] = '\0';   // Set an end of string at this point

                    counter++;
                    if (counter == RAYGUI_TEXTSPLIT_MAX_ITEMS) break;
                }
            }

            *count = counter;

            return result;
        }

        // Convert color data from RGB to HSV
        // NOTE: Color data should be passed normalized
        raylib::Vector3 RayGui::ConvertRGBtoHSV(raylib::Vector3 rgb)
        {
            raylib::Vector3 hsv = { 0 };
            float min = 0.0f;
            float max = 0.0f;
            float delta = 0.0f;

            min = (rgb.x < rgb.y)? rgb.x : rgb.y;
            min = (min < rgb.z)? min  : rgb.z;

            max = (rgb.x > rgb.y)? rgb.x : rgb.y;
            max = (max > rgb.z)? max  : rgb.z;

            hsv.z = max;            // Value
            delta = max - min;

            if (delta < 0.00001f)
            {
                hsv.y = 0.0f;
                hsv.x = 0.0f;           // Undefined, maybe NAN?
                return hsv;
            }

            if (max > 0.0f)
            {
                // NOTE: If max is 0, this divide would cause a crash
                hsv.y = (delta/max);    // Saturation
            }
            else
            {
                // NOTE: If max is 0, then r = g = b = 0, s = 0, h is undefined
                hsv.y = 0.0f;
                hsv.x = 0.0f;           // Undefined, maybe NAN?
                return hsv;
            }

            // NOTE: Comparing float values could not work properly
            if (rgb.x >= max) hsv.x = (rgb.y - rgb.z)/delta;    // Between yellow & magenta
            else
            {
                if (rgb.y >= max) hsv.x = 2.0f + (rgb.z - rgb.x)/delta;  // Between cyan & yellow
                else hsv.x = 4.0f + (rgb.x - rgb.y)/delta;      // Between magenta & cyan
            }

            hsv.x *= 60.0f;     // Convert to degrees

            if (hsv.x < 0.0f) hsv.x += 360.0f;

            return hsv;
        }

        // Convert color data from HSV to RGB
        // NOTE: Color data should be passed normalized
        raylib::Vector3 RayGui::ConvertHSVtoRGB(raylib::Vector3 hsv)
        {
            raylib::Vector3 rgb = { 0 };
            float hh = 0.0f, p = 0.0f, q = 0.0f, t = 0.0f, ff = 0.0f;
            long i = 0;

            // NOTE: Comparing float values could not work properly
            if (hsv.y <= 0.0f)
            {
                rgb.x = hsv.z;
                rgb.y = hsv.z;
                rgb.z = hsv.z;
                return rgb;
            }

            hh = hsv.x;
            if (hh >= 360.0f) hh = 0.0f;
            hh /= 60.0f;

            i = (long)hh;
            ff = hh - i;
            p = hsv.z*(1.0f - hsv.y);
            q = hsv.z*(1.0f - (hsv.y*ff));
            t = hsv.z*(1.0f - (hsv.y*(1.0f - ff)));

            switch (i)
            {
                case 0:
                {
                    rgb.x = hsv.z;
                    rgb.y = t;
                    rgb.z = p;
                } break;
                case 1:
                {
                    rgb.x = q;
                    rgb.y = hsv.z;
                    rgb.z = p;
                } break;
                case 2:
                {
                    rgb.x = p;
                    rgb.y = hsv.z;
                    rgb.z = t;
                } break;
                case 3:
                {
                    rgb.x = p;
                    rgb.y = q;
                    rgb.z = hsv.z;
                } break;
                case 4:
                {
                    rgb.x = t;
                    rgb.y = p;
                    rgb.z = hsv.z;
                } break;
                case 5:
                default:
                {
                    rgb.x = hsv.z;
                    rgb.y = p;
                    rgb.z = q;
                } break;
            }

            return rgb;
        }

        // Scroll bar control (used by GuiScrollPanel())
        int RayGui::GuiScrollBar(raylib::Rectangle bounds, int value, int minValue, int maxValue)
        {
            GuiState state = guiState;

            // Is the scrollbar horizontal or vertical?
            bool isVertical = (bounds.width > bounds.height) ? false : true;

            // The size (width or height depending on scrollbar type) of the spinner buttons
            const int spinnerSize = GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE) ? (isVertical ? (int)bounds.width - 2*GuiGetStyle(GuiControl::SCROLLBAR, GuiControlProperty::BORDER_WIDTH) : (int)bounds.height - 2*GuiGetStyle(GuiControl::SCROLLBAR, GuiControlProperty::BORDER_WIDTH)) : 0;

            // Arrow buttons [<] [>] [∧] [∨]
            raylib::Rectangle arrowUpLeft = { 0 };
            raylib::Rectangle arrowDownRight = { 0 };

            // Actual area of the scrollbar excluding the arrow buttons
            raylib::Rectangle scrollbar = { 0 };

            // Slider bar that moves     --[///]-----
            raylib::Rectangle slider = { 0 };

            // Normalize value
            if (value > maxValue) value = maxValue;
            if (value < minValue) value = minValue;

            const int range = maxValue - minValue;
            int sliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);

            // Calculate rectangles for all of the components
            arrowUpLeft = raylib::Rectangle { (float)bounds.x + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), (float)bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };

            if (isVertical)
            {
                arrowDownRight = raylib::Rectangle { (float)bounds.x + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), (float)bounds.y + bounds.height - spinnerSize - GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
                scrollbar = raylib::Rectangle { bounds.x + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), arrowUpLeft.y + arrowUpLeft.height, bounds.width - 2*(GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)), bounds.height - arrowUpLeft.height - arrowDownRight.height - 2*GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) };
                sliderSize = (sliderSize >= scrollbar.height) ? ((int)scrollbar.height - 2) : sliderSize;     // Make sure the slider won't get outside of the scrollbar
                slider = raylib::Rectangle { (float)bounds.x + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING), (float)scrollbar.y + (int)(((float)(value - minValue)/range)*(scrollbar.height - sliderSize)), (float)bounds.width - 2*(GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)), (float)sliderSize };
            }
            else
            {
                arrowDownRight = raylib::Rectangle { (float)bounds.x + bounds.width - spinnerSize - GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), (float)bounds.y + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
                scrollbar = raylib::Rectangle { arrowUpLeft.x + arrowUpLeft.width, bounds.y + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), bounds.width - arrowUpLeft.width - arrowDownRight.width - 2*GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), bounds.height - 2*(GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)) };
                sliderSize = (sliderSize >= scrollbar.width) ? ((int)scrollbar.width - 2) : sliderSize;       // Make sure the slider won't get outside of the scrollbar
                slider = raylib::Rectangle { (float)scrollbar.x + (int)(((float)(value - minValue)/range)*(scrollbar.width - sliderSize)), (float)bounds.y + GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING), (float)sliderSize, (float)bounds.height - 2*(GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)) };
            }

            // Update control
            //--------------------------------------------------------------------
            if ((state != GuiState::STATE_DISABLED) && !guiLocked)
            {
                raylib::Vector2 mousePoint = raylib::GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, bounds))
                {
                    state = STATE_FOCUSED;

                    // Handle mouse wheel
                    int wheel = (int)raylib::GetMouseWheelMove();
                    if (wheel != 0) value += wheel;

                    if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON))
                    {
                        if (CheckCollisionPointRec(mousePoint, arrowUpLeft)) value -= range/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                        else if (CheckCollisionPointRec(mousePoint, arrowDownRight)) value += range/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);

                        state = STATE_PRESSED;
                    }
                    else if (raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON))
                    {
                        if (!isVertical)
                        {
                            raylib::Rectangle scrollArea = { arrowUpLeft.x + arrowUpLeft.width, arrowUpLeft.y, scrollbar.width, bounds.height - 2*GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH) };
                            if (CheckCollisionPointRec(mousePoint, scrollArea)) value = (int)(((float)(mousePoint.x - scrollArea.x - slider.width/2)*range)/(scrollArea.width - slider.width) + minValue);
                        }
                        else
                        {
                            raylib::Rectangle scrollArea = { arrowUpLeft.x, arrowUpLeft.y+arrowUpLeft.height, bounds.width - 2*GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH),  scrollbar.height };
                            if (CheckCollisionPointRec(mousePoint, scrollArea)) value = (int)(((float)(mousePoint.y - scrollArea.y - slider.height/2)*range)/(scrollArea.height - slider.height) + minValue);
                        }
                    }
                }

                // Normalize value
                if (value > maxValue) value = maxValue;
                if (value < minValue) value = minValue;
            }
            //--------------------------------------------------------------------

            // Draw control
            //--------------------------------------------------------------------
            GuiDrawRectangle(bounds, GuiGetStyle(SCROLLBAR, GuiControlProperty::BORDER_WIDTH), Fade(raylib::GetColor(GuiGetStyle(GuiControl::LISTVIEW, BORDER + state*3)), guiAlpha), Fade(raylib::GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_DISABLED)), guiAlpha));   // Draw the background

            GuiDrawRectangle(scrollbar, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL)), guiAlpha));     // Draw the scrollbar active area background
            GuiDrawRectangle(slider, 0, raylib::BLANK, Fade(raylib::GetColor(GuiGetStyle(SLIDER, BORDER + state*3)), guiAlpha));         // Draw the slider bar

            // Draw arrows (using icon if available)
            if (GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE))
            {
        #if defined(RAYGUI_NO_ICONS)
                GuiDrawText(isVertical ? "^" : "<", raylib::Rectangle{ arrowUpLeft.x, arrowUpLeft.y, isVertical ? bounds.width : bounds.height, isVertical ? bounds.width : bounds.height },
                    TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))), guiAlpha));
                GuiDrawText(isVertical ? "v" : ">", raylib::Rectangle{ arrowDownRight.x, arrowDownRight.y, isVertical ? bounds.width : bounds.height, isVertical ? bounds.width : bounds.height },
                    TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))), guiAlpha));
        #else
                GuiDrawText(isVertical ? "#121#" : "#118#", raylib::Rectangle{ arrowUpLeft.x, arrowUpLeft.y, isVertical ? bounds.width : bounds.height, isVertical ? bounds.width : bounds.height },
                    TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)), guiAlpha));   // ICON_ARROW_UP_FILL / ICON_ARROW_LEFT_FILL
                GuiDrawText(isVertical ? "#120#" : "#119#", raylib::Rectangle{ arrowDownRight.x, arrowDownRight.y, isVertical ? bounds.width : bounds.height, isVertical ? bounds.width : bounds.height },
                    TEXT_ALIGN_CENTER, Fade(raylib::GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)), guiAlpha));   // ICON_ARROW_DOWN_FILL / ICON_ARROW_RIGHT_FILL
        #endif
            }
            //--------------------------------------------------------------------

            return value;
        }


    }  

}

#endif
