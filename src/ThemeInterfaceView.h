/*
 * Copyright 2000-2008, François Revol, <revol@free.fr>. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <View.h>

namespace Z {
namespace ThemeManager {
	class ThemeManager;
} // ns ThemeManager
} // ns Z
using Z::ThemeManager::ThemeManager;

class BBitmap;
class BSeparator;
class BBox;
class BListView;
class BButton;
class BScrollView;
class BTabView;
class BTextControl;
class BTextView;
class BMessage;
class ScreenshotView;
class BStringView;
class MyInvoker;

class ThemeInterfaceView : public BView
{
public:
							ThemeInterfaceView(BRect _bounds);
		virtual 			~ThemeInterfaceView();
		
		virtual void 		AllAttached();
		virtual void 		MessageReceived(BMessage* _msg);
		ThemeManager*		GetThemeManager();
		
		void				PopulateThemeList();
		void				PopulateAddonList();

		status_t			Revert();
		status_t			ApplyDefaults();
		status_t			ApplySelected();
		status_t			CreateNew(const char *name);
		status_t			SaveSelected();
		status_t			DeleteSelected();
		status_t			AddScreenshot();
		
		status_t			ThemeSelected();
		
		void				SetIsRevertable();
		void				SetScreenshot(BBitmap *shot);
		status_t			AError(const char *func, status_t err);

private:
		static int32		_ThemeListPopulatorTh(void *arg);
		void				_ThemeListPopulator();
		
		ThemeManager*		fThemeManager;
		bool				fScreenshotPaneHidden;
		bool				fHasScreenshot;
		
		BScrollView*		fThemeListSV;
		BListView*			fThemeList;
		BButton*			fDefaultsBtn;
		BButton*			fApplyBtn;
		BButton*			fNewBtn;
		BTextControl*		fNameText;
		BButton*			fSaveBtn;
		BButton*			fDeleteBtn;
		BButton*			fSetShotBtn;
		BButton*			fMoreThemesBtn;
		BTabView*			fTabView;
		ScreenshotView*		fScreenshotPane;
		BStringView*		fScreenshotText;
		BView*				fScreenshotTab;
		BTextView*			fDetails;
		BScrollView*		fAddonListSV;
		BListView*			fAddonList;
};

extern "C" BView *themes_pref(const BRect& Bounds);

#define SSPANE_WIDTH 320
#define SSPANE_HEIGHT 240


