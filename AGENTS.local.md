<!-- Repo-specific appendix to the shared AGENTS.md. Generic conventions live in AGENTS_ARCH.md (hardlinked). -->

```markdown
# ksf_FA_DynamicPricing
## Architecture Overview
This repository implements **Dynamic Pricing Engine** extracted from WooCommerce Dynamic Pricing plugin - supports bulk discounts, role-based pricing, category discounts, and BOGO deals.
## Repository Structure
```
ksf_FA_DynamicPricing/
├── sql/                    # Database schemas (FA TB_PREF tables)
│   ├── fa_pricing_rules.sql
│   ├── fa_pricing_conditions.sql
│   └── fa_pricing_applications.sql
├── includes/              # FA-specific DB classes
│   ├── pricing_rules_db.inc
│   ├── pricing_conditions_db.inc
│   └── ...
├── src/                    # Business logic (namespace: Ksf\FA\DynamicPricing\)
│   ├── Contracts/        # Interfaces
│   ├── Services/         # Pricing engine services
│   └── Rules/            # Rule strategy implementations
├── pages/                 # UI pages (FA admin)
├── hooks.php
├── composer.json
└── ProjectDocs/
    ├── Requirements.md
    ├── RTM.md
    ├── BABOK.md
    └── UML.md
```
## Dependencies
- **ksf_FA_DynamicPricing_Core** (business logic - extracted from WooCommerce)
- **ksf_FA_CRM** (customer contacts for role-based pricing)
- **FrontAccounting 2.4+**
```
