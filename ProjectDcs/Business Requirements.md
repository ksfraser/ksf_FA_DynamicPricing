# ksf_FA_DynamicPricing - Business Requirements Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Approved

---

## 1. Introduction

### 1.1 Purpose

The FA_DynamicPricing module provides a dynamic pricing engine for FrontAccounting, enabling businesses to automatically adjust prices based on rules such as bulk discounts, customer role-based pricing, category-based discounts, and Buy-One-Get-One (BOGO) promotions. This functionality was extracted from WooCommerce Dynamic Pricing plugin and adapted for the FrontAccounting platform.

### 1.2 Problem Statement

Organizations selling products through FrontAccounting need flexible pricing strategies beyond fixed pricing. Current limitations include:

- No support for volume-based discounts
- Inability to offer role-based pricing tiers
- Manual price adjustments required
- No automation for promotional pricing
- Limited support for complex discount combinations

### 1.3 Scope

This module provides:

1. **Rule-Based Pricing** - Define pricing rules with conditions
2. **Bulk Discounts** - Quantity-based price adjustments
3. **Role-Based Pricing** - Customer type-based pricing
4. **Category Discounts** - Product category pricing rules
5. **BOGO Deals** - Buy-one-get-one promotions
6. **Stacking Rules** - Combine multiple pricing rules

---

## 2. Module Overview

### 2.1 Core Features

| Feature | Description | Priority |
|---------|-------------|----------|
| Pricing Rules Engine | Central engine for evaluating and applying rules | Critical |
| Bulk Discounts | Tiered pricing based on quantity | Critical |
| Role-Based Pricing | Different prices for customer types | High |
| Category Discounts | Discounts on product categories | High |
| BOGO Promotions | Buy X Get Y free or discounted | High |
| Rule Prioritization | Define priority for conflicting rules | Medium |
| Date-Based Rules | Time-limited promotional pricing | Medium |
| Combinable Rules | Stack multiple discounts | Medium |

### 2.2 Pricing Rule Types

#### Volume-Based Discounts
```
Example: Buy 10+ get 15% off
         Buy 50+ get 25% off
```

#### Role-Based Pricing
```
Example: Wholesale customers get 20% off
         VIP customers get 30% off
```

#### Category Discounts
```
Example: Electronics category: 10% off
         Seasonal items: 25% off
```

#### BOGO Rules
```
Example: Buy 2 Get 1 Free
         Buy 3 Get 2nd at 50% off
```

### 2.3 Rule Conditions

| Condition Type | Description |
|----------------|-------------|
| Quantity | Minimum items in cart |
| Customer Role | Customer type (wholesale, retail, VIP) |
| Product Category | Category of items being purchased |
| Date Range | Promotion valid dates |
| Order Total | Minimum order amount |

### 2.4 Rule Actions

| Action Type | Description |
|-------------|-------------|
| Percentage Discount | X% off item/category/cart |
| Fixed Discount | $X off item/category/cart |
| Fixed Price | Set price to $X |
| BOGO | Buy X Get Y free/discounted |

---

## 3. User Stories

### 3.1 Sales Manager

> As a Sales Manager, I want to create tiered bulk discounts so that customers are incentivized to buy more.

**Acceptance Criteria:**
- Can create quantity-based pricing rules
- Can define multiple discount tiers
- Rules apply automatically at checkout
- Can set rule validity periods

### 3.2 Wholesale Manager

> As a Wholesale Manager, I want to offer special pricing to wholesale customers so that they receive their contracted rates automatically.

**Acceptance Criteria:**
- Can assign customer roles via CRM
- Can create role-based pricing rules
- Wholesale prices applied automatically
- Retail customers see standard pricing

### 3.3 Marketing Manager

> As a Marketing Manager, I want to run category-wide promotions so that I can drive sales in specific product areas.

**Acceptance Criteria:**
- Can create category-based discounts
- Can set promotion duration
- Multiple categories can have active promotions
- Promotions exclude excluded products

### 3.4 Customer Service Representative

> As a Customer Service Rep, I want to verify dynamic pricing is applied correctly so that customers receive correct prices.

**Acceptance Criteria:**
- Can view applied pricing rules on invoices
- Can explain price adjustments
- Can identify which rules applied

---

## 4. Integration Dependencies

### 4.1 Required Modules

| Module | Dependency Type | Purpose |
|--------|-----------------|---------|
| ksf_FA_CRM | Required | Customer contacts and role assignment |
| FrontAccounting Core | Required | Sales, inventory, UI framework |
| ksf_FA_Coupons | Optional | Combine with coupon discounts |

### 4.2 Optional Integrations

| Integration | Purpose |
|-------------|---------|
| ksf_FA_Forms | Customer feedback collection |
| ksf_FA_Fleet | Vehicle pricing (special pricing) |

### 4.3 Data Dependencies

| External Table | Relationship | Purpose |
|---------------|--------------|---------|
| `{PREFIX}debtors` | Via CRM | Customer master data |
| `{PREFIX}stock_master` | Direct | Product information |
| `{PREFIX}stock_category` | Direct | Category classification |
| `{PREFIX}person_types` | Via CRM | Customer roles |

---

## 5. Database Schema

### 5.1 Primary Tables

#### `fa_pricing_rules`
Stores pricing rule definitions.

| Column | Type | Description |
|--------|------|-------------|
| `rule_id` | INT (PK) | Primary key |
| `rule_name` | VARCHAR(100) | Rule name/description |
| `rule_type` | ENUM | bulk, role, category, bogo, cart |
| `conditions` | JSON | Rule conditions |
| `action` | JSON | Discount action details |
| `priority` | INT | Rule priority (lower = higher) |
| `stackable` | TINYINT(1) | Can combine with other rules |
| `valid_from` | DATE | Start date |
| `valid_to` | DATE | End date |
| `is_active` | TINYINT(1) | Active flag |
| `created_at` | TIMESTAMP | Creation timestamp |

#### `fa_pricing_conditions`
Stores condition configurations for rules.

| Column | Type | Description |
|--------|------|-------------|
| `condition_id` | INT (PK) | Primary key |
| `rule_id` | INT (FK) | Parent rule |
| `condition_type` | VARCHAR(50) | Type of condition |
| `condition_value` | JSON | Condition parameters |

#### `fa_pricing_applications`
Logs applied pricing rules.

| Column | Type | Description |
|--------|------|-------------|
| `application_id` | INT (PK) | Primary key |
| `rule_id` | INT (FK) | Applied rule |
| `invoice_id` | INT | Invoice reference |
| `line_item_id` | INT | Line item reference |
| `discount_amount` | DECIMAL(15,2) | Discount applied |
| `applied_at` | TIMESTAMP | Application timestamp |

---

## 6. Security Model

### 6.1 Security Areas

| Area | Code | Description |
|------|------|-------------|
| SS_DYNPRICE | 118 << 8 | Dynamic Pricing section |
| SA_DYNPRICEVIEW | SS_DYNPRICE \| 1 | View pricing rules |
| SA_DYNPRICECREATE | SS_DYNPRICE \| 2 | Create pricing rules |
| SA_DYNPRICEMANAGE | SS_DYNPRICE \| 3 | Edit/delete rules |

### 6.2 Access Control

- View access: Sales, Marketing, Finance
- Create/Edit: Marketing, Admin
- Delete: Admin only

---

## 7. Success Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| Rule evaluation time | < 100ms | Performance testing |
| Discount accuracy | 100% | Reconciliation |
| Rule creation time | < 3 minutes | User testing |
| Compatible products | 100% | Inventory check |

---

## 8. Future Enhancements

1. **Customer-Specific Rules** - Individual customer pricing
2. **Product-Specific Exclusions** - Exclude certain products from rules
3. **Layered Discounts** - Multiple discount types combined
4. **API for External Systems** - Integration with e-commerce
5. **Price Lock** - Lock prices for customers during session
6. **Tiered Membership Pricing** - Membership level pricing