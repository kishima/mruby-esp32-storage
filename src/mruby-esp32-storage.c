#include <mruby.h>
#include <mruby/value.h>
#include <stdio.h>
#include <string.h>

#include "diskio.h"
#include "ff.h"
#include "esp_vfs_fat.h"
#include "esp_task_wdt.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_spiffs.h"
#include "soc/efuse_reg.h"

#include "mruby-esp32-storage.h"

void (*suspend_interrupts)(void) = NULL;
void (*enable_interrupts)(void) = NULL;

static int spiffs_mounted = 0;
static char spiffs_mount_path[16];

mrb_value mrb_esp32_storage_mount_spiffs(mrb_state *mrb, mrb_value self)
{
  char* mount_path;
  mrb_int path_len;
  mrb_int max_file;
  mrb_bool format;
  mrb_bool opt;
  mrb_get_args(mrb, "s|ib?", &mount_path,&path_len,&max_file,&format,&opt);

  if(path_len>15){
    mrb_raise(mrb,E_ARGUMENT_ERROR,"mount path is too long");
  }
  printf(">%s:%d\n",mount_path,path_len);
  memcpy(spiffs_mount_path,mount_path,path_len);
  spiffs_mount_path[path_len]='\0';

  if(!opt){
    max_file = 1;
    format = true;
  }

  esp_vfs_spiffs_conf_t conf = {
      .base_path              = spiffs_mount_path,
      .partition_label        = NULL,
      .max_files              = max_file,
      .format_if_mount_failed = format
  };
  mrb_storage_user_interrupt_suspend();
  spiffs_mounted = (esp_vfs_spiffs_register(&conf) == ESP_OK);
  mrb_storage_user_interrupt_enbale();

  if(!spiffs_mounted)
  {
    mrb_raise(mrb,E_RUNTIME_ERROR,"mount failed");
  }

  return self;
}

mrb_value mrb_esp32_storage_umount_spiffs(mrb_state *mrb, mrb_value self)
{
  return self;
}



void
mrb_mruby_esp32_storage_gem_init(mrb_state* mrb) {
  struct RClass *esp32_module = mrb_define_module(mrb, "ESP32");

  //Storage module
  struct RClass *esp32_storage_module = mrb_define_module_under(mrb, esp32_module, "Storage");

  mrb_define_module_function(mrb, esp32_storage_module, "mount_spiffs", mrb_esp32_storage_mount_spiffs, MRB_ARGS_ARG(1,2));
  mrb_define_module_function(mrb, esp32_storage_module, "umount_spiffs", mrb_esp32_storage_umount_spiffs, MRB_ARGS_NONE());

  //FILE class

}

void
mrb_mruby_esp32_storage_gem_final(mrb_state* mrb) {
}
