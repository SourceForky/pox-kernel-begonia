#!/usr/bin/env bash
# Fixups for the v4.16,v4.17,v4.18,v4.19,v4.20,v5.0 escalation step (base branch onyx-v4.15).
#
# Starts empty by design: everything earlier steps needed is already committed in
# onyx-v4.15.  Each entry below restores a file this tree already ships (a revert can
# never remove a driver or a file); additive PORTs are done with python3.
set +e
echo "===== v4.16,v4.17,v4.18,v4.19,v4.20,v5.0 fixups ====="

REVERT_TO_OURS="
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

echo "===== fixups done ====="
git status --short | head -30
