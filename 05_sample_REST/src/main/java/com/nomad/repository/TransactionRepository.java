package com.nomad.repository;

import com.nomad.entity.Transaction;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface TransactionRepository extends JpaRepository<Transaction, Long> {

    // Find by customer ID
    List<Transaction> findByCustomerId(Integer customerId);

    // Find by customer name
    List<Transaction> findByCustomerNameContainingIgnoreCase(String customerName);

    // Find valid transactions only
    List<Transaction> findByIsValidTrue();

    // Find transactions by amount range
    List<Transaction> findByAmountBetween(Integer minAmount, Integer maxAmount);

    // Custom query to find transactions by customer ID and valid status
    @Query("SELECT t FROM Transaction t WHERE t.customerId = :customerId AND t.isValid = :isValid")
    List<Transaction> findByCustomerIdAndValidStatus(@Param("customerId") Integer customerId,
            @Param("isValid") Boolean isValid);

    // Custom query to get total amount by customer
    @Query("SELECT SUM(t.amount) FROM Transaction t WHERE t.customerId = :customerId AND t.isValid = true")
    Integer getTotalAmountByCustomer(@Param("customerId") Integer customerId);
}