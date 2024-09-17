#include "rptr_grid/rptr_grid.hpp"

#include <godot_cpp/godot.hpp>

namespace rptr::gdextension
{
    void initialize_extension(godot::ModuleInitializationLevel p_level)
    {
        if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
        {
            return;
        }

        godot::ClassDB::register_class<RptrGrid>();
    }

    void terminate_extension(godot::ModuleInitializationLevel p_level)
    {
        if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
        {
            return;
        }
    }

    extern "C"
    {
        GDExtensionBool rptr_library_init(
            GDExtensionInterfaceGetProcAddress p_get_proc_address,
            const GDExtensionClassLibraryPtr p_library,
            GDExtensionInitialization *r_initialization)
        {
            godot::GDExtensionBinding::InitObject init_obj(
                p_get_proc_address, p_library, r_initialization);

            init_obj.register_initializer(initialize_extension);
            init_obj.register_terminator(terminate_extension);
            init_obj.set_minimum_library_initialization_level(
                godot::MODULE_INITIALIZATION_LEVEL_SCENE);

            return init_obj.init();
        }
    }

} //namespace rptr::gdextension
