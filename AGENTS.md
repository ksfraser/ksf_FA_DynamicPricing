# AGENTS.md - ksf_FA_DynamicPricing#

## Architecture Overview#

This repository implements **Dynamic Pricing Engine** extracted from WooCommerce Dynamic Pricing plugin - supports bulk discounts, role-based pricing, category discounts, and BOGO deals.

### Core Principles#
- **SOLID**: Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, Dependency Inversion#
- **DRY**: Don't Repeat Yourself - extract reusable logic#
- **TDD**: Test-Driven Development - write tests first#
- **DI**: Dependency Injection - inject dependencies, don't hardcode#
- **SRP**: Single Responsibility Principle - each class has one reason to change#

## Repository Structure#

```
ksf_FA_DynamicPricing/
├── sql/                    # Database schemas (FA TB_PREF tables)#
│   ├── fa_pricing_rules.sql#
│   ├── fa_pricing_conditions.sql#
│   └── fa_pricing_applications.sql#
├── includes/              # FA-specific DB classes#
│   ├── pricing_rules_db.inc#
│   ├── pricing_conditions_db.inc#
│   └── ...#
├── src/                    # Business logic (namespace: Ksf\FA\DynamicPricing\)#
│   ├── Contracts/        # Interfaces#
│   ├── Services/         # Pricing engine services#
│   └── Rules/            # Rule strategy implementations#
├── pages/                 # UI pages (FA admin)#
├── hooks.php#
├── composer.json#
└── ProjectDocs/#
    ├── Requirements.md#
    ├── RTM.md#
    ├── BABOK.md#
    └── UML.md#
```

## Dependencies#

- **ksf_FA_DynamicPricing_Core** (business logic - extracted from WooCommerce)#
- **ksf_FA_CRM** (customer contacts for role-based pricing)#
- **FrontAccounting 2.4+**#

## Development Workflow

All development is done in the **devel tree** (`~/Documents/ksf_FA_DynamicPricing`). Do **not** edit files in the UAT bind point directly.

### Workflow Steps
1. **Develop** in this repo (feature branches preferred)
2. **Test**: run repo-appropriate tests
3. **Lint**: `php -l` on modified PHP files (no syntax errors)
4. **Commit** and **Push** branch to GitHub
5. **Merge** to `master` when ready
6. **Push** `master` to GitHub
7. **Deploy** to UAT by pulling in the Infrastructure bind point:

   ```
   cd ~/ksf_Infrastructure/fa_modules/ksf_FA_DynamicPricing
   git stash -u
   git pull origin master
   git stash pop
   ```

### UAT Bind Point
| Path | Purpose |
|------|---------|
| `~/Documents/ksf_FA_DynamicPricing` | Devel tree — all development, testing, commits |
| `~/ksf_Infrastructure/fa_modules/ksf_FA_DynamicPricing` | UAT bind point — deployment target, integration testing (if mirrored) |

