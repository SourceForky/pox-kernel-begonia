#!/usr/bin/env bash
# Fixups for the v4.16 escalation step (base branch onyx-v4.15).
#
# Everything earlier steps needed is already committed in onyx-v4.15, so this file
# starts from the one cluster v4.16 introduces.  Each entry below restores a file
# this tree already ships; a revert can never remove a driver or a file.
set +e
echo "===== v4.16 fixups ====="

# ---------------------------------------------------------------------------
# Fix 1: scripts/kconfig must stay at ONE generation.
#
# The merge adopted upstream's kconfig files one by one, which left the host
# .config parser half-upgraded:
#
#   ours : symbol.c  menu.c  confdata.c  expr.c   (the merge kept these)
#   up   : conf.c  Makefile  lkc.h  lkc_proto.h  util.c  kconf_id.c  ...
#
# That cannot compile.  Clang (run 39263099678):
#
#   symbol.c:906:13: error: conflicting types for 'sym_expand_string_value';
#                    have 'const char *(const char *)'
#   lkc_proto.h:34:7: note: previous declaration of 'sym_expand_string_value'
#                     with type 'char *(const char *)'
#   zconf.tab.c:1555:9: warning: implicit declaration of function 'menu_end_entry'
#
# and because kconfig is a *host* tool it fails before a single kernel object is
# compiled ("object compiles seen: 0").
#
# kconfig is self-contained and host-only: no kernel code and no driver depends
# on which generation builds the parser, and our generation is complete (the
# tree ran make defconfig with it for every version until now).  So hold the
# WHOLE directory at our generation instead of cherry-picking two headers and
# leaving the rest mixed.
#
# The general mechanism for this is self-heal rule 6; this list pre-seeds it so
# the v4.16 build is clean on the first attempt.
# ---------------------------------------------------------------------------
REVERT_TO_OURS="
scripts/kconfig/.gitignore
scripts/kconfig/Makefile
scripts/kconfig/conf.c
scripts/kconfig/kconf_id.c
scripts/kconfig/kxgettext.c
scripts/kconfig/lkc.h
scripts/kconfig/lkc_proto.h
scripts/kconfig/lxdialog/check-lxdialog.sh
scripts/kconfig/mconf.c
scripts/kconfig/util.c
scripts/kconfig/zconf.lex.c_shipped
scripts/kconfig/zconf.tab.c_shipped
"

for f in $REVERT_TO_OURS; do
  if ! git cat-file -e "origin/onyx-v4.15:$f" 2>/dev/null; then
    echo "  [-] not in ours, skipped: $f"
  elif git checkout origin/onyx-v4.15 -- "$f" 2>/dev/null; then
    echo "  [+] reverted to ours: $f"
  else
    echo "  [!] could not revert $f"
  fi
done

# Sanity: the two declarations that clashed must be gone.
if grep -q 'char \*sym_expand_string_value' scripts/kconfig/lkc_proto.h 2>/dev/null; then
  echo "  [!] lkc_proto.h still declares upstream's sym_expand_string_value"
else
  echo "  [=] lkc_proto.h matches our symbol.c"
fi
if grep -q 'void menu_end_entry(void);' scripts/kconfig/lkc.h 2>/dev/null; then
  echo "  [=] lkc.h declares menu_end_entry (what our zconf.tab.c calls)"
else
  echo "  [!] lkc.h does not declare menu_end_entry"
fi

echo "===== fixups done ====="
git status --short | head -30
