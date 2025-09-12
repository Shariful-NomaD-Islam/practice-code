package com.nomad.controller;

import com.nomad.entity.Transaction;
import com.nomad.service.TransactionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/v1")
public class SampleRestController {

    @Autowired
    private TransactionService transactionService;

    @GetMapping("/hello")
    public String hello() {
        return "Hello, World!";
    }

    @GetMapping("/status")
    public String status() {
        return "Service is running!";
    }

    // Get all transactions
    @GetMapping("/transactions")
    public List<Transaction> getAllTransactions() {
        return transactionService.getAllTransactions();
    }

    // Get transaction by ID
    @GetMapping("/transactions/{id}")
    public ResponseEntity<Transaction> getTransactionById(@PathVariable Long id) {
        Optional<Transaction> transaction = transactionService.getTransactionById(id);
        return transaction.map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
    }

    // Get transactions by customer ID
    @GetMapping("/transactions/customer/{customerId}")
    public List<Transaction> getTransactionsByCustomerId(@PathVariable Integer customerId) {
        return transactionService.getTransactionsByCustomerId(customerId);
    }

    // Get valid transactions only
    @GetMapping("/transactions/valid")
    public List<Transaction> getValidTransactions() {
        return transactionService.getValidTransactions();
    }

    // Search transactions by customer name
    @GetMapping("/transactions/search")
    public List<Transaction> searchTransactionsByCustomerName(@RequestParam String customerName) {
        return transactionService.searchByCustomerName(customerName);
    }

    // Get transactions by amount range
    @GetMapping("/transactions/amount-range")
    public List<Transaction> getTransactionsByAmountRange(
            @RequestParam Integer minAmount,
            @RequestParam Integer maxAmount) {
        return transactionService.getTransactionsByAmountRange(minAmount, maxAmount);
    }

    // Create new transaction
    @PostMapping("/transactions")
    public Transaction createTransaction(@RequestBody Transaction transaction) {
        return transactionService.createTransaction(transaction);
    }

    // Update transaction
    @PutMapping("/transactions/{id}")
    public ResponseEntity<Transaction> updateTransaction(@PathVariable Long id, @RequestBody Transaction transaction) {
        try {
            Transaction updatedTransaction = transactionService.updateTransaction(id, transaction);
            return ResponseEntity.ok(updatedTransaction);
        } catch (RuntimeException e) {
            return ResponseEntity.notFound().build();
        }
    }

    // Delete transaction
    @DeleteMapping("/transactions/{id}")
    public ResponseEntity<Void> deleteTransaction(@PathVariable Long id) {
        Optional<Transaction> transaction = transactionService.getTransactionById(id);
        if (transaction.isPresent()) {
            transactionService.deleteTransaction(id);
            return ResponseEntity.ok().build();
        }
        return ResponseEntity.notFound().build();
    }

    // Get total amount by customer
    @GetMapping("/transactions/customer/{customerId}/total")
    public Integer getTotalAmountByCustomer(@PathVariable Integer customerId) {
        return transactionService.getTotalAmountByCustomer(customerId);
    }
}