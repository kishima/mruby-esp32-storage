#include <mruby.h>
#include <mruby/value.h>

#include <stdio.h>


mrb_value mrb_esp32_storage_mount_spiffs(mrb_state *mrb, mrb_value self)
{

}

void
mrb_mruby_esp32_system_gem_init(mrb_state* mrb) {
  struct RClass *esp32_module = mrb_define_module(mrb, "ESP32");

  struct RClass *esp32_storage_module = mrb_define_module_under(mrb, esp32_module, "Storage");

  mrb_define_module_function(mrb, esp32_storage_module, "mound", mrb_esp32_storage_mount_spiffs, MRB_ARGS_REQ(1));
}

void
mrb_mruby_esp32_system_gem_final(mrb_state* mrb) {
}
