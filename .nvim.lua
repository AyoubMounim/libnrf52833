
local root_dir = vim.fn.expand('%:p:h')
local script_dir = root_dir .. '/scripts/'

local cmd_compile = string.format(':!%s/compile<CR>', script_dir)
local cmd_install = string.format(':!%s/install<CR>', script_dir)
local cmd_embed = string.format(':!%s/embed<CR>', script_dir)

vim.keymap.set('n', '<c-f1>', cmd_compile)
vim.keymap.set('n', '<c-f3>', cmd_install)
vim.keymap.set('n', '<c-f4>', cmd_embed)

