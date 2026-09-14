# ksf_FA_DynamicPricing - Functional Requirements Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Approved

---

## 1. Introduction

### 1.1 Purpose

This document details the functional requirements for the FA_DynamicPricing module.

### 1.2 Scope

- Pricing rule CRUD operations
- Rule condition evaluation
- Discount calculation and application
- Integration with sales/invoice modules

---

## 2. Functional Requirements

### 2.1 Rule Management

#### FR-DP-001: Create Pricing Rule

**Description:** Create a new pricing rule with conditions and actions.

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| rule_name | VARCHAR(100) | Yes | Rule display name |
| rule_type | ENUM | Yes | bulk, role, category, bogo, cart |
| conditions | JSON | Yes | Rule conditions |
| action | JSON | Yes | Discount action |
| priority | INT | No | Rule priority (default 100) |
| stackable | TINYINT(1) | No | Can combine with others |
| valid_from | DATE | No | Start date |
| valid_to | DATE | No | End date |

**Business Rules:**
- rule_name must be unique
- Conditions must be valid for rule_type
- Action must be valid for rule_type

**Acceptance Criteria:**
- [ ] Can create bulk discount rule
- [ ] Can create category discount rule
- [ ] Can create role-based rule
- [ ] Can create BOGO rule
- [ ] System validates all inputs

---

#### FR-DP-002: Edit Pricing Rule

**Description:** Modify existing pricing rule parameters.

**Editable Fields:**
- rule_name
- conditions
- action
- priority
- stackable
- valid_from
- valid_to
- is_active

**Non-Editable:**
- rule_type (after creation)

**Acceptance Criteria:**
- [ ] Can modify all editable fields
- [ ] Changes take effect immediately
- [ ] Existing applications preserved

---

#### FR-DP-003: Delete Pricing Rule

**Description:** Remove pricing rule from system.

**Business Rules:**
- Rule with existing applications can be deleted
- Application history preserved for audit

**Acceptance Criteria:**
- [ ] Rule deleted successfully
- [ ] Application records remain
- [ ] Rule no longer evaluates

---

#### FR-DP-004: View Pricing Rules

**Description:** Display all pricing rules with filtering.

**View Columns:**
- Rule Name
- Type
- Priority
- Status (Active/Inactive)
- Valid From
- Valid To

**Filters:**
- Status
- Type
- Date Range

**Acceptance Criteria:**
- [ ] All rules displayed
- [ ] Filters work correctly
- [ ] Sorting available

---

### 2.2 Condition Types

#### FR-DP-005: Quantity Condition

**Description:** Rule triggers based on item quantity.

**Condition Structure:**
```json
{
  "type": "quantity",
  "min_quantity": 10,
  "max_quantity": 100
}
```

**Acceptance Criteria:**
- [ ] Min quantity validation works
- [ ] Max quantity works with min
- [ ] Multiple tiers supported

---

#### FR-DP-006: Category Condition

**Description:** Rule triggers for specific product categories.

**Condition Structure:**
```json
{
  "type": "category",
  "category_ids": [5, 10, 15]
}
```

**Acceptance Criteria:**
- [ ] Single category works
- [ ] Multiple categories work
- [ ] Subcategory handling correct

---

#### FR-DP-007: Customer Role Condition

**Description:** Rule triggers for specific customer types.

**Condition Structure:**
```json
{
  "type": "customer_role",
  "roles": ["wholesale", "vip", "preferred"]
}
```

**Acceptance Criteria:**
- [ ] Single role works
- [ ] Multiple roles work
- [ ] Role lookup from CRM correct

---

#### FR-DP-008: Date Range Condition

**Description:** Rule triggers within specific dates.

**Condition Structure:**
```json
{
  "type": "date_range",
  "valid_from": "2026-06-01",
  "valid_to": "2026-08-31"
}
```

**Acceptance Criteria:**
- [ ] Date validation works
- [ ] Past dates rejected for new rules
- [ ] Range calculation correct

---

#### FR-DP-009: Order Total Condition

**Description:** Rule triggers based on cart total.

**Condition Structure:**
```json
{
  "type": "order_total",
  "min_total": 100.00,
  "max_total": 1000.00
}
```

**Acceptance Criteria:**
- [ ] Minimum total works
- [ ] Maximum total works
- [ ] Range works

---

### 2.3 Action Types

#### FR-DP-010: Percentage Discount Action

**Description:** Apply percentage discount.

**Action Structure:**
```json
{
  "type": "percentage",
  "value": 15
}
```

**Calculation:** `discount = line_total * (value / 100)`

**Acceptance Criteria:**
- [ ] Discount calculates correctly
- [ ] Value can be 0-100
- [ ] Rounds to 2 decimals

---

#### FR-DP-011: Fixed Amount Discount Action

**Description:** Apply fixed dollar discount.

**Action Structure:**
```json
{
  "type": "fixed",
  "value": 25.00
}
```

**Acceptance Criteria:**
- [ ] Fixed amount applied
- [ ] Cannot exceed line total
- [ ] Currency formatting correct

---

#### FR-DP-012: Fixed Price Action

**Description:** Set item to fixed price.

**Action Structure:**
```json
{
  "type": "fixed_price",
  "value": 49.99
}
```

**Acceptance Criteria:**
- [ ] Item price set to fixed value
- [ ] Works regardless of original price

---

#### FR-DP-013: BOGO Action

**Description:** Buy X Get Y free/discounted.

**Action Structure:**
```json
{
  "type": "bogo",
  "buy_quantity": 2,
  "get_quantity": 1,
  "get_discount": 100
}
```

**Calculation:**
- Buy 2, Get 1 free: get_discount = 100
- Buy 3, Get 2nd at 50%: get_discount = 50

**Acceptance Criteria:**
- [ ] Free item calculated correctly
- [ ] Partial discount calculated correctly
- [ ] Multiple BOGO in cart handled

---

### 2.4 Rule Evaluation

#### FR-DP-014: Evaluate All Rules

**Description:** Evaluate all applicable rules for a cart/order.

**Evaluation Flow:**
1. Load active rules
2. Sort by priority
3. Evaluate each rule against cart items
4. Apply non-conflicting rules
5. Handle conflicting rules by priority

**Acceptance Criteria:**
- [ ] All active rules evaluated
- [ ] Priority order respected
- [ ] Correct discounts applied

---

#### FR-DP-015: Handle Rule Conflicts

**Description:** Resolve conflicts when multiple rules apply.

**Resolution Rules:**
1. Higher priority (lower number) wins
2. If stackable, rules combine
3. Maximum stackable rules configurable

**Acceptance Criteria:**
- [ ] Non-stackable: only highest priority applies
- [ ] Stackable: rules combine
- [ ] Max stack limit enforced

---

#### FR-DP-016: Cache Rules

**Description:** Cache rules for performance.

**Cache Strategy:**
- Cache active rules in memory
- Invalidate on rule changes
- TTL: 1 hour (configurable)

**Acceptance Criteria:**
- [ ] Rules cached after first load
- [ ] Cache invalidated on changes
- [ ] Performance improved

---

### 2.5 Integration

#### FR-DP-017: Sales Order Integration

**Description:** Apply dynamic pricing to sales orders.

**Integration Points:**
1. Sales order creation - evaluate rules
2. Line item display - show original and discounted price
3. Order total - reflect all discounts

**Acceptance Criteria:**
- [ ] Pricing evaluated on order creation
- [ ] Discounts shown on order
- [ ] Total reflects discounts

---

#### FR-DP-018: Invoice Integration

**Description:** Apply dynamic pricing to invoices.

**Integration Points:**
1. Invoice creation from sales order
2. Discount line items
3. Audit trail of applied rules

**Acceptance Criteria:**
- [ ] Invoice reflects order pricing
- [ ] Discounts itemized
- [ ] Rule application logged

---

#### FR-DP-019: CRM Integration

**Description:** Access customer roles for pricing.

**Integration:**
- Customer role lookup via CRM
- Role-based rule evaluation

**Acceptance Criteria:**
- [ ] Customer roles retrieved
- [ ] Role-based rules apply
- [ ] Default role for unknown customers

---

## 3. Non-Functional Requirements

### 3.1 Performance

| Metric | Target |
|--------|--------|
| Rule evaluation | < 100ms per cart |
| Cache hit | < 10ms |
| Rule CRUD | < 500ms |

### 3.2 Scalability

- Support 1000+ active rules
- Handle 500+ concurrent evaluations
- Efficient for large carts (100+ items)

---

## 4. Requirements Traceability

| Requirement ID | Source | Priority | Status |
|----------------|--------|----------|--------|
| FR-DP-001 to FR-DP-004 | BR-001 | Critical | Pending |
| FR-DP-005 to FR-DP-009 | BR-002 | High | Pending |
| FR-DP-010 to FR-DP-013 | BR-003 | High | Pending |
| FR-DP-014 to FR-DP-016 | BR-004 | Critical | Pending |
| FR-DP-017 to FR-DP-019 | BR-005 | Critical | Pending |