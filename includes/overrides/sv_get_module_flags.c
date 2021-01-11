/*
From SunVox docs, module flags are:

SV_MODULE_FLAG_EXISTS;
SV_MODULE_FLAG_EFFECT;
SV_MODULE_FLAG_MUTE;
SV_MODULE_FLAG_SOLO;
SV_MODULE_FLAG_BYPASS;
SV_MODULE_INPUTS_OFF;
SV_MODULE_INPUTS_MASK;
SV_MODULE_OUTPUTS_OFF;
SV_MODULE_OUTPUTS_MASK;
*/

static int flags[] = {
  SV_MODULE_FLAG_EXISTS,
  SV_MODULE_FLAG_EFFECT,
  SV_MODULE_FLAG_MUTE,
  SV_MODULE_FLAG_SOLO,
  SV_MODULE_FLAG_BYPASS,
  SV_MODULE_INPUTS_OFF,
  SV_MODULE_INPUTS_MASK,
  SV_MODULE_OUTPUTS_OFF,
  SV_MODULE_OUTPUTS_MASK,
};

static char* flags_str[] = {
  "flag_exists",
  "flag_effect",
  "flag_mute",
  "flag_solo",
  "flag_bypass",
  "inputs_off",
  "inputs_mask",
  "outputs_off",
  "outputs_mask",
};

static int flags_count = 9;

static int lua_sv_get_module_flags(lua_State *L) {
  int lua_slot = (int)luaL_checknumber(L, 1);
  int lua_mod_num = (int)luaL_checknumber(L, 2);
  uint32_t module_flags = sv_get_module_flags(lua_slot, lua_mod_num);
  lua_newtable(L); // table with values mapped to flags

  for (int i = 0; i < flags_count; i++) {
    if (module_flags & flags[i]) {
      lua_pushstring(L, flags_str[i]);
      lua_pushboolean(L, 1);
      lua_settable(L, -3);
    }
  }

  return 1;
}
