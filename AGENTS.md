# PS5 maintenance preferences

- Prefer the PS5 Tailscale address for every device operation. Keep actual addresses and credentials outside tracked files.
- After each maintenance operation, synchronize changed scripts/configuration and a sanitized result record to this GitHub repository. Do not publish raw logs, account details, game files, tokens, or device identifiers.
- Do not update PS5 firmware or enable automatic system updates.
- Do not sign into or intentionally use Sony/PSN network services. Preserve/update blocking where possible, while retaining Tailscale access.
- Distinguish installed DNS filtering from verified system-wide blocking. Do not claim complete Sony isolation without router-level enforcement and validation.
- Latest clarification: do not deploy DNS-based blocking or network interception. nanoDNS was stopped and removed from autoload at the user's request. Discuss non-DNS alternatives before implementing a new network policy.
