# ksf_FA_DynamicPricing - Architecture Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Approved

---

## 1. Architecture Overview

### 1.1 Module Purpose

The FA_DynamicPricing module provides a FrontAccounting platform adapter for the dynamic pricing engine. The core business logic is extracted from WooCommerce Dynamic Pricing plugin and adapted for FrontAccounting's architecture.

### 1.2 Architecture Pattern

The module follows the **Business Logic + Platform Adapter** pattern:

```
ksf_FA_DynamicPricing/         # Platform-specific UI & DB adapters
    └── Ksfraser\FA\DynamicPricing\
```

This separation allows the pricing engine core to be reused across platforms.

---

## 2. Component Architecture

### 2.1 Module Structure

```
ksf_FA_DynamicPricing/
├── sql/                          # Database schemas
│   ├── fa_pricing_rules.sql      # Rule definitions
│   ├── fa_pricing_conditions.sql # Condition configs
│   └── fa_pricing_applications.sql # Application logs
├── includes/                     # FA-specific database classes
│   ├── pricing_rules_db.inc     # Rule CRUD operations
│   ├── pricing_conditions_db.inc # Condition data access
│   └── pricing_applications_db.inc # Application logging
├── src/                         # Business logic (future)
│   ├── Contracts/              # Interfaces
│   ├── Services/               # Pricing engine services
│   └── Rules/                  # Rule strategy implementations
├── pages/                       # FA admin UI pages
├── hooks.php                    # FA module hooks
└── ProjectDcs/                 # Project documentation
```

---

## 3. Class Diagram

```
┌─────────────────────────────────────────────────────────┐
│                   FA Module Layer                        │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │     hooks_fa_dynamic_pricing                     │   │
│  │     extends hooks                               │   │
│  ├─────────────────────────────────────────────────┤   │
│  │  + install_options($app): void                  │   │
│  │  + install_access(): array                      │   │
│  │  + activate_extension(): bool                   │   │
│  └─────────────────────────────────────────────────┘   │
│                          │                               │
│                          ▼                               │
│  ┌─────────────────────────────────────────────────┐   │
│  │              FA UI Layer                         │   │
│  ├─────────────────────────────────────────────────┤   │
│  │  pricing_rules.php                             │   │
│  │  - Rule CRUD pages                             │   │
│  │  - Reports and analytics                        │   │
│  └─────────────────────────────────────────────────┘   │
│                          │                               │
│                          ▼                               │
│  ┌─────────────────────────────────────────────────┐   │
│  │            FA Database Layer                    │   │
│  ├─────────────────────────────────────────────────┤   │
│  │  pricing_rules_db.inc                           │   │
│  │  pricing_conditions_db.inc                       │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
└─────────────────────────────────────────────────────────┘
          │                    ▲
          │                    │
          ▼                    │
┌─────────────────────────────────────────────────────────┐
│              Business Logic Layer (Core)                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │  PricingEngine                                  │   │
│  │  - evaluateRules(): PricingResult[]             │   │
│  │  - applyRule(): Discount                       │   │
│  └─────────────────────────────────────────────────┘   │
│           │                    │                      │
│           ▼                    ▼                      │
│  ┌─────────────────┐  ┌─────────────────────────┐   │
│  │ BulkDiscountRule│  │ CategoryDiscountRule    │   │
│  └─────────────────┘  └─────────────────────────┘   │
│  ┌─────────────────┐  ┌─────────────────────────┐   │
│  │ RoleBasedRule   │  │ BOGORule               │   │
│  └─────────────────┘  └─────────────────────────┘   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 4. Database Architecture

### 4.1 Entity Relationship Diagram

```
┌─────────────────────┐         ┌─────────────────────────┐
│  fa_pricing_rules   │         │ fa_pricing_conditions   │
├─────────────────────┤         ├─────────────────────────┤
│ PK rule_id          │──────┐  │ PK condition_id         │
│    rule_name        │      │  │ FK rule_id         ─────┘
│    rule_type        │      │  │    condition_type
│    conditions (JSON)│      │  │    condition_value (JSON)
│    action (JSON)    │      │  └─────────────────────────┘
│    priority         │      │
│    stackable        │      │
│    valid_from       │      │
│    valid_to         │      │
│    is_active        │      │
└─────────────────────┘      │
         │                   │
         │                   ▼
         │         ┌─────────────────────────┐
         │         │    fa_pricing_applications│
         │         ├─────────────────────────┤
         │         │ PK application_id       │
         └────────►│ FK rule_id              │
                   │    invoice_id           │
                   │    line_item_id          │
                   │    discount_amount       │
                   │    applied_at            │
                   └─────────────────────────┘

         │
         ▼
┌─────────────────────┐
│  FA Sales Linkage   │
├─────────────────────┤
│  - sales_orders     │
│  - sales_order_details│
│  - invoices         │
└─────────────────────┘
```

---

## 5. Pricing Engine Architecture

### 5.1 Rule Evaluation Flow

```
[Invoice/Sales Order]
        │
        ▼
[Load Active Rules]
        │
        ▼
[Evaluate Conditions] ──► [Match?]
        │                       │
        │ No                    │ Yes
        ▼                       ▼
[Next Rule]           [Calculate Discount]
                              │
                              ▼
                      [Check Stackability]
                              │
              ┌───────────────┴───────────────┐
              │                               │
        [Stackable]                    [Not Stackable]
              │                               │
              ▼                               ▼
      [Combine with                        [Apply Only
       Other Rules]                        This Rule]
              │                               │
              └───────────────┬───────────────┘
                              │
                              ▼
                    [Update Line Item Price]
```

### 5.2 Rule Priority Resolution

When multiple rules apply to the same item:

1. **Priority Order**: Lower priority number = higher priority
2. **Exclusive Mode**: Highest priority rule wins
3. **Stackable Mode**: Rules combine (up to defined limit)

```
Rule Priority Example:
- Rule A (Priority 10): 15% off category
- Rule B (Priority 20): 10% off all items
- Rule C (Priority 5): $5 off item

If item matches A and B:
- Non-stackable: Rule A applies (15%)
- Stackable: Rule A + Rule B apply

If item matches all three:
- Non-stackable: Rule C applies ($5)
- Stackable: Rule C + Rule A + Rule B apply
```

---

## 6. Rule Type Implementations

### 6.1 Bulk Discount Rule

```php
class BulkDiscountRule implements PricingRuleInterface {
    // Conditions: { "min_quantity": 10, "max_quantity": 50 }
    // Action: { "type": "percentage", "value": 15 }
    
    public function evaluate(CartItem $item, Context $context): ?Discount {
        if ($item->quantity >= $this->conditions['min_quantity']) {
            if ($item->quantity <= $this->conditions['max_quantity'] ?? PHP_INT_MAX) {
                return new Discount(
                    type: 'percentage',
                    value: $this->action['value'],
                    amount: $item->unit_price * $item->quantity * ($this->action['value'] / 100)
                );
            }
        }
        return null;
    }
}
```

### 6.2 Category Discount Rule

```php
class CategoryDiscountRule implements PricingRuleInterface {
    // Conditions: { "category_id": 5 }
    // Action: { "type": "percentage", "value": 10 }
    
    public function evaluate(CartItem $item, Context $context): ?Discount {
        if ($item->category_id === $this->conditions['category_id']) {
            return new Discount(
                type: 'percentage',
                value: $this->action['value'],
                amount: $item->line_total * ($this->action['value'] / 100)
            );
        }
        return null;
    }
}
```

### 6.3 Role-Based Rule

```php
class RoleBasedDiscountRule implements PricingRuleInterface {
    // Conditions: { "customer_roles": ["wholesale", "vip"] }
    // Action: { "type": "percentage", "value": 20 }
    
    public function evaluate(CartItem $item, Context $context): ?Discount {
        $customer_role = $context->getCustomerRole();
        if (in_array($customer_role, $this->conditions['customer_roles'])) {
            return new Discount(
                type: 'percentage',
                value: $this->action['value'],
                amount: $item->line_total * ($this->action['value'] / 100)
            );
        }
        return null;
    }
}
```

### 6.4 BOGO Rule

```php
class BOGORule implements PricingRuleInterface {
    // Conditions: { "buy_quantity": 2 }
    // Action: { "type": "bogo", "get_quantity": 1, "get_discount": 100 }
    
    public function evaluate(CartItem $item, Context $context): ?Discount {
        $buy_qty = $this->conditions['buy_quantity'];
        $free_qty = $this->action['get_quantity'];
        $free_item_count = floor($item->quantity / ($buy_qty + $free_qty)) * $free_qty;
        
        if ($free_item_count > 0) {
            $discount_amount = $item->unit_price * $free_item_count * ($this->action['get_discount'] / 100);
            return new Discount(
                type: 'bogo',
                value: $this->action['get_discount'],
                amount: $discount_amount
            );
        }
        return null;
    }
}
```

---

## 7. Extension Integration

### 7.1 Hook Points

```php
// In hooks_fa_dynamic_pricing

// Pre-calculation hook
$hooks->add_hook('pricing_pre_calculate', function($cart) {
    // Modify cart before pricing
});

// Post-calculation hook
$hooks->add_hook('pricing_post_calculate', function($discounts) {
    // Log or modify results
});

// Rule evaluation hook
$hooks->add_hook('pricing_evaluate_rule', function($rule, $item) {
    // Custom rule logic
});
```

---

## 8. Configuration

### 8.1 Module Settings

| Setting | Default | Description |
|---------|---------|-------------|
| default_priority | 100 | Default rule priority |
| max_stackable | 3 | Maximum stackable rules |
| cache_ttl | 3600 | Rule cache lifetime |
| log_applications | true | Log rule applications |

---

## 9. Performance Considerations

### 9.1 Caching Strategy

1. **Rule Cache**: Cache active rules in memory
2. **Category Cache**: Cache category mappings
3. **Customer Role Cache**: Cache customer role assignments

### 9.2 Optimization

- Only evaluate rules for affected items
- Use database indexes on rule conditions
- Batch rule evaluation for multiple items

---

## 10. Future Architecture (KSF II)

For future versions:

```
┌─────────────────────────────────────────────────────────┐
│               FrontAccounting Adapter                   │
├─────────────────────────────────────────────────────────┤
│  hooks_fa_dynamic_pricing                               │
└────────────────────────┬────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────┐
│              Core Business Layer                        │
├─────────────────────────────────────────────────────────┤
│  Ksfraser\DynamicPricing\Services\PricingEngine         │
│  Ksfraser\DynamicPricing\Rules\RuleRegistry             │
│  Ksfraser\DynamicPricing\Contracts\RuleInterface         │
└─────────────────────────────────────────────────────────┘
```

---

## 11. Technology Stack

| Component | Technology | Version |
|-----------|------------|---------|
| Framework | FrontAccounting | 2.4+ |
| Database | MySQL/MariaDB | 5.7+ |
| PHP | PHP | 7.3+ |
| Caching | APCu (optional) | Latest |