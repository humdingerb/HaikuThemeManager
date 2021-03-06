/*
 * Copyright 2000-2008, François Revol, <revol@free.fr>. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

/*
 * deskbar ThemesAddon class
 */

#include <Alert.h>
#include <Application.h>
#include <Directory.h>
#include <Message.h>
#include <Messenger.h>
#include <Roster.h>
#ifdef __HAIKU__
#include <Deskbar.h>
#else
#include <be_apps/Deskbar/Deskbar.h>
#endif

#include <string.h>

#include "ThemesAddon.h"
#include "UITheme.h"

#ifdef SINGLE_BINARY
#define instantiate_themes_addon instantiate_themes_addon_deskbar
#endif

#define A_NAME "Deskbar"
#define A_MSGNAME Z_THEME_DESKBAR_SETTINGS
#define A_DESCRIPTION "Deskbar on-screen position"


class DeskbarThemesAddon : public ThemesAddon {
public:
	DeskbarThemesAddon();
	~DeskbarThemesAddon();
	
const char *Description();

status_t	RunPreferencesPanel();

status_t	AddNames(BMessage &names);

status_t	ApplyTheme(BMessage &theme, uint32 flags=0L);
status_t	MakeTheme(BMessage &theme, uint32 flags=0L);

status_t	ApplyDefaultTheme(uint32 flags=0L);
};


DeskbarThemesAddon::DeskbarThemesAddon()
	: ThemesAddon(A_NAME, A_MSGNAME)
{
}


DeskbarThemesAddon::~DeskbarThemesAddon()
{
}


const char *
DeskbarThemesAddon::Description()
{
	return A_DESCRIPTION;
}


status_t
DeskbarThemesAddon::RunPreferencesPanel()
{
	if (be_roster->Launch("application/x-vnd.Haiku-DeskbarPreferences") == B_OK)
		return B_OK;

	status_t err;
	entry_ref ref;
	BEntry ent;
	err = ent.SetTo("/boot/beos/preferences/Deskbar");
	if (!err) {
		err = ent.GetRef(&ref);
		if (!err) {
			err = be_roster->Launch(&ref);
		}
	}
	if (!err)
		return B_OK;
	err = ent.SetTo("/system/add-ons/Preferences/Deskbar");
	if (!err) {
		err = ent.GetRef(&ref);
		if (!err) {
			err = be_roster->Launch(&ref);
			if (err) {
				BMessage msg(B_REFS_RECEIVED);
				msg.AddRef("refs", &ref);
				be_app_messenger.SendMessage(&msg);
			}
		}
	}
	return err;
}


status_t
DeskbarThemesAddon::AddNames(BMessage &names)
{
	names.AddString(Z_THEME_DESKBAR_SETTINGS, "Deskbar position");
	names.AddString("db:location", "Deskbar on-screen position");
	names.AddString("db:expanded", "Deskbar is expanded");
	names.AddString("db:iconSize", "Deskbar icons size");
	names.AddString("db:hideLabels", "Deskbar hide labels");
	return B_OK;
}


status_t
DeskbarThemesAddon::ApplyTheme(BMessage &theme, uint32 flags)
{
	BMessage deskbar;
	status_t err;
	int32 loc = 5;
	bool expanded = true;
	BDeskbar db;
	
	if (!(flags & UI_THEME_SETTINGS_SET_ALL) || !(AddonFlags() & Z_THEME_ADDON_DO_SET_ALL))
		return B_OK;
	
	err = MyMessage(theme, deskbar);
	if (err)
		return err;
	
	if (deskbar.FindInt32("db:location", &loc) != B_OK)
		return ENOENT;
	deskbar.FindBool("db:expanded", &expanded);
	return db.SetLocation((deskbar_location)loc, expanded);
}


status_t
DeskbarThemesAddon::MakeTheme(BMessage &theme, uint32 flags)
{
	BMessage deskbar;
	status_t err;
	
	(void)flags;
	err = MyMessage(theme, deskbar);
	if (err)
		deskbar.MakeEmpty();
	
	deskbar_location loc;
	bool expanded;
	BDeskbar db;

	deskbar.RemoveName("db:location");
	deskbar.RemoveName("db:expanded");

	loc = db.Location(&expanded);
	deskbar.AddInt32("db:location", (int32)loc);
	deskbar.AddBool("db:expanded", expanded);
	
	err = SetMyMessage(theme, deskbar);
	return err;
}


status_t
DeskbarThemesAddon::ApplyDefaultTheme(uint32 flags)
{
	BMessage theme;
	BMessage deskbar;
	deskbar_location loc = B_DESKBAR_RIGHT_TOP;
	bool expanded = true;
	int32 iconSize = 16;
	bool hideLabels = false;
	deskbar.AddInt32("db:location", (int32)loc);
	deskbar.AddBool("db:expanded", expanded);
	deskbar.AddInt32("db:iconSize", iconSize);
	deskbar.AddBool("db:hideLabels", hideLabels);
	theme.AddMessage(A_MSGNAME, &deskbar);
	return ApplyTheme(theme, flags);
}


ThemesAddon *
instantiate_themes_addon()
{
	return (ThemesAddon *) new DeskbarThemesAddon;
}

