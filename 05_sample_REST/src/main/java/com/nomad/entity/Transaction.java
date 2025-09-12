package com.nomad.entity;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "transaction")
public class Transaction {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "pk_id")
    private Long pkId;

    @Column(name = "customer_id")
    private Integer customerId;

    @Column(name = "customer_name", length = 100)
    private String customerName;

    @Column(name = "amount")
    private Integer amount;

    @Column(name = "create_time")
    private LocalDateTime createTime;

    @Column(name = "is_valid")
    private Boolean isValid;

    // Constructors
    public Transaction() {
    }

    public Transaction(Integer customerId, String customerName, Integer amount) {
        this.customerId = customerId;
        this.customerName = customerName;
        this.amount = amount;
        this.createTime = LocalDateTime.now();
        this.isValid = true;
    }

    // Getters and Setters
    public Long getPkId() {
        return pkId;
    }

    public void setPkId(Long pkId) {
        this.pkId = pkId;
    }

    public Integer getCustomerId() {
        return customerId;
    }

    public void setCustomerId(Integer customerId) {
        this.customerId = customerId;
    }

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public Integer getAmount() {
        return amount;
    }

    public void setAmount(Integer amount) {
        this.amount = amount;
    }

    public LocalDateTime getCreateTime() {
        return createTime;
    }

    public void setCreateTime(LocalDateTime createTime) {
        this.createTime = createTime;
    }

    public Boolean getIsValid() {
        return isValid;
    }

    public void setIsValid(Boolean isValid) {
        this.isValid = isValid;
    }

    @Override
    public String toString() {
        return "Transaction{" +
                "pkId=" + pkId +
                ", customerId=" + customerId +
                ", customerName='" + customerName + '\'' +
                ", amount=" + amount +
                ", createTime=" + createTime +
                ", isValid=" + isValid +
                '}';
    }
}