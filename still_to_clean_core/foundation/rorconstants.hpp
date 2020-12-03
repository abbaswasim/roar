#pragma once

#include <foundation/rortypes.h>

namespace ror
{
#ifndef log
#	define log ror::Log::get_instance()
#endif

#ifndef font
#	define font ror::FontManager::get_instance()
#endif

#ifndef fontdefault
#	define fontdefault ror::FontManager::get_instance()->get_default_font()
#endif

#ifndef keyboard
#	define keyboard application::Keyboard::get_instance()
#endif

#ifndef mouse
#	define mouse application::Mouse::get_instance()
#endif

#ifndef gui
#	define gui ror::cguimanager::get_instance()
#endif

#ifndef string
#	define string ror::StringTable::get_instance()->get_string
#endif
}        // namespace ror
