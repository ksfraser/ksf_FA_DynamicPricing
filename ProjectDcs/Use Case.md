# ksf_FA_DynamicPricing - Use Case Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Approved

---

## 1. Introduction

### 1.1 Purpose

This document captures all use cases for the FA_DynamicPricing module.

### 1.2 Actor Definitions

| Actor | Description | Access Level |
|-------|-------------|--------------|
| Marketing Manager | Creates and manages pricing rules | SA_DYNPRICECREATE |
| Sales Manager | Reviews pricing effectiveness | SA_DYNPRICEVIEW |
| System Administrator | Full access to all functions | SA_DYNPRICEMANAGE |
| Sales Representative | Benefits from automatic pricing | SA_DYNPRICEVIEW |
| System | Evaluates and applies rules | Automatic |

---

## 2. Use Case Catalog

### 2.1 Rule Management Use Cases

| Use Case ID | Use Case Name |
|-------------|---------------|
| UC-DP-001 | Create Bulk Discount Rule |
| UC-DP-002 | Create Category Discount Rule |
| UC-DP-003 | Create Role-Based Discount Rule |
| UC-DP-004 | Create BOGO Promotion Rule |
| UC-DP-005 | Edit Pricing Rule |
| UC-DP-006 | Delete Pricing Rule |
| UC-DP-007 | Activate/Deactivate Rule |
| UC-DP-008 | View Pricing Rules |

### 2.2 Rule Evaluation Use Cases

| Use Case ID | Use Case Name |
|-------------|---------------|
| UC-DP-009 | Evaluate Cart Pricing |
| UC-DP-010 | Apply Pricing to Sales Order |
| UC-DP-011 | Handle Rule Conflicts |

### 2.3 Reporting Use Cases

| Use Case ID | Use Case Name |
|-------------|---------------|
| UC-DP-012 | View Rule Application History |
| UC-DP-013 | Generate Pricing Analytics |

---

## 3. Detailed Use Cases

---

### UC-DP-001: Create Bulk Discount Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Marketing Manager creates new volume discount

**Preconditions:**
1. User authenticated with SA_DYNPRICECREATE
2. Product/category exists for rule

**Basic Flow:**
```
1. Navigate to Pricing Rules > Create Rule
2. Enter rule name: "Volume Discount 10+"
3. Select rule type: Bulk Discount
4. Configure conditions:
   - Min quantity: 10
   - Max quantity: (empty for unlimited)
5. Configure action:
   - Type: Percentage
   - Value: 15
6. Set priority: 50
7. Set stackable: No
8. Set validity: 2026-06-01 to 2026-08-31
9. Save rule
10. Verify rule appears in list as Active
```

**Alternative Flow:**

A1: Duplicate Rule Name
```
9a. Rule name already exists
9b. System shows error: "Rule name already exists"
9c. User enters unique name
9d. Continue
```

---

### UC-DP-002: Create Category Discount Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Marketing creates category promotion

**Basic Flow:**
```
1. Navigate to Pricing Rules > Create Rule
2. Enter rule name: "Electronics Summer Sale"
3. Select rule type: Category Discount
4. Configure conditions:
   - Category: Electronics (ID: 5)
5. Configure action:
   - Type: Percentage
   - Value: 10
6. Set validity: 2026-06-01 to 2026-08-31
7. Save
```

---

### UC-DP-003: Create Role-Based Discount Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Wholesale pricing setup

**Basic Flow:**
```
1. Create rule for wholesale customers
2. Select rule type: Role-Based
3. Configure conditions:
   - Customer roles: [wholesale]
4. Configure action:
   - Type: Percentage
   - Value: 20
5. Set as ongoing (no end date)
6. Save
```

---

### UC-DP-004: Create BOGO Promotion Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Create "Buy 2 Get 1 Free" promotion

**Basic Flow:**
```
1. Navigate to create rule
2. Enter name: "BOGO - Buy 2 Get 1 Free"
3. Select rule type: BOGO
4. Configure conditions:
   - Applies to: Category or specific products
5. Configure action:
   - Buy quantity: 2
   - Get quantity: 1
   - Get discount: 100%
6. Set validity: 2 weeks
7. Save
```

---

### UC-DP-005: Edit Pricing Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Need to adjust rule parameters

**Basic Flow:**
```
1. Select rule from list
2. Click "Edit"
3. Modify desired fields
4. Save changes
5. Verify changes reflected
```

---

### UC-DP-006: Delete Pricing Rule

**Primary Actor:** System Administrator  
**Trigger:** Rule no longer needed

**Basic Flow:**
```
1. Select rule
2. Click "Delete"
3. Confirm deletion
4. Verify rule removed
5. Application history preserved
```

---

### UC-DP-007: Activate/Deactivate Rule

**Primary Actor:** Marketing Manager  
**Trigger:** Toggle rule status

**Basic Flow:**
```
1. Select rule
2. Toggle status (Active/Inactive)
3. System updates immediately
4. Deactivated rules not evaluated
```

---

### UC-DP-008: View Pricing Rules

**Primary Actor:** All Users  
**Trigger:** Navigate to rules list

**Basic Flow:**
```
1. Navigate to Pricing Rules page
2. View all rules with columns:
   - Name, Type, Priority, Status, Valid dates
3. Apply filters as needed
4. Sort by clicking column headers
```

---

### UC-DP-009: Evaluate Cart Pricing

**Primary Actor:** System (automatic)  
**Trigger:** Cart/sales order submitted

**Basic Flow:**
```
1. System receives cart/sales order
2. System loads active rules (from cache if available)
3. For each line item:
   a. Evaluate all matching rules
   b. Sort by priority
   c. Apply highest priority (or stackable)
4. Calculate total discounts
5. Return pricing result
6. Cache rules for next evaluation
```

**Alternative Flow:**

A1: No Rules Match
```
3a. No rules match current cart
3b. Return original pricing
3c. No discount applied
```

A2: Multiple Matching Rules
```
3a. Multiple rules match
3b. Apply priority ordering
3c. If conflicting (non-stackable), use highest priority
3d. If stackable, combine up to max limit
```

---

### UC-DP-010: Apply Pricing to Sales Order

**Primary Actor:** System  
**Trigger:** Sales order finalized

**Basic Flow:**
```
1. Sales order created with items
2. System evaluates pricing rules
3. Discounts calculated for each item
4. Discount line items added to order
5. Order total updated
6. Application logged
7. Order saved with pricing
```

---

### UC-DP-011: Handle Rule Conflicts

**Primary Actor:** System  
**Trigger:** Conflicting rules match

**Resolution Logic:**
```
1. Multiple rules match same item/cart
2. Sort by priority (ascending)
3. Check if rules are stackable
4. If not stackable:
   - Apply only highest priority rule
5. If stackable:
   - Apply all matching rules
   - Stop at max_stackable limit
6. Log which rules applied
```

---

### UC-DP-012: View Rule Application History

**Primary Actor:** Sales Manager, Finance  
**Trigger:** Review rule performance

**Basic Flow:**
```
1. Navigate to Reports > Rule Applications
2. Select rule (or all rules)
3. Set date range
4. View list of applications:
   - Invoice/Order reference
   - Customer
   - Discount amount
   - Applied at timestamp
5. Export to CSV if needed
```

---

### UC-DP-013: Generate Pricing Analytics

**Primary Actor:** Sales Manager  
**Trigger:** Need pricing performance data

**Basic Flow:**
```
1. Navigate to Reports > Pricing Analytics
2. Select date range
3. System aggregates:
   - Total discounts given
   - Most used rules
   - Rule effectiveness (orders using rules)
   - Revenue impact
4. Display charts and summary
```

---

## 4. Use Case Matrix

| Actor | UC-DP-001 | UC-DP-002 | UC-DP-003 | UC-DP-004 | UC-DP-005 | UC-DP-006 | UC-DP-007 | UC-DP-008 | UC-DP-009 | UC-DP-010 | UC-DP-011 | UC-DP-012 | UC-DP-013 |
|-------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|------------|
| Marketing Manager | ● | ● | ● | ● | ● | ○ | ● | ● | ○ | ○ | ○ | ● | ● |
| Sales Manager | ○ | ○ | ○ | ○ | ○ | ○ | ○ | ● | ○ | ○ | ○ | ● | ● |
| System Admin | ○ | ○ | ○ | ○ | ● | ● | ● | ● | ○ | ○ | ○ | ● | ● |
| Sales Rep | ○ | ○ | ○ | ○ | ○ | ○ | ○ | ● | ○ | ○ | ○ | ○ | ○ |
| System | ○ | ○ | ○ | ○ | ○ | ○ | ○ | ○ | ● | ● | ● | ○ | ○ |

● = Primary actor, ○ = Secondary actor

---

## 5. Error Handling

| Error | Cause | Resolution |
|-------|-------|------------|
| Rule evaluation timeout | Too many rules | Optimize, increase timeout |
| Circular rule dependency | Rules reference each other | Prevent in validation |
| Invalid condition format | JSON malformed | Validate on save |
| Missing required field | Field not provided | Show validation error |