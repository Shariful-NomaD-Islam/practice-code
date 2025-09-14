package com.nomad.controller;

import com.nomad.entity.Transaction;
import com.nomad.model.Reply;
import com.nomad.service.TransactionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/v1")
public class SampleRestController {

    @Autowired
    private TransactionService transactionService;

    @GetMapping("/hello")
    public Reply<String> hello() {
        return Reply.success("Hello, World!");
    }

    @GetMapping("/status")
    public Reply<String> status() {
        return Reply.success("Service is running!");
    }

    // Get all transactions
    @GetMapping("/transactions")
    public Reply<List<Transaction>> getAllTransactions() {
        List<Transaction> transactions = transactionService.getAllTransactions();
        return Reply.success(transactions);
    }

    // Get transaction by ID
    @GetMapping("/transactions/{id}")
    public Reply<Transaction> getTransactionById(@PathVariable Long id) {
        Optional<Transaction> transaction = transactionService.getTransactionById(id);
        return transaction.map(Reply::success).orElse(Reply.notFound());
    }

    // Get transactions by customer ID
    @GetMapping("/transactions/customer/{customerId}")
    public Reply<List<Transaction>> getTransactionsByCustomerId(@PathVariable Integer customerId) {
        List<Transaction> transactions = transactionService.getTransactionsByCustomerId(customerId);
        return Reply.success(transactions);
    }

    // Get valid transactions only
    @GetMapping("/transactions/valid")
    public Reply<List<Transaction>> getValidTransactions() {
        List<Transaction> transactions = transactionService.getValidTransactions();
        return Reply.success(transactions);
    }

    // Search transactions by customer name
    @GetMapping("/transactions/search")
    public Reply<List<Transaction>> searchTransactionsByCustomerName(@RequestParam String customerName) {
        if (customerName == null || customerName.trim().isEmpty()) {
            return Reply.badRequest("Customer name parameter is required");
        }
        List<Transaction> transactions = transactionService.searchByCustomerName(customerName);
        return Reply.success(transactions);
    }

    // Get transactions by amount range
    @GetMapping("/transactions/amount-range")
    public Reply<List<Transaction>> getTransactionsByAmountRange(
            @RequestParam Integer minAmount,
            @RequestParam Integer maxAmount) {
        if (minAmount < 0 || maxAmount < 0 || minAmount > maxAmount) {
            return Reply.badRequest("Invalid amount range: minAmount and maxAmount must be positive and minAmount <= maxAmount");
        }
        List<Transaction> transactions = transactionService.getTransactionsByAmountRange(minAmount, maxAmount);
        return Reply.success(transactions);
    }

    // Create new transaction
    @PostMapping("/transactions")
    public Reply<Transaction> createTransaction(@RequestBody Transaction transaction) {
        try {
            Transaction createdTransaction = transactionService.createTransaction(transaction);
            return Reply.success(createdTransaction);
        } catch (Exception e) {
            return Reply.serverError("Failed to create transaction: " + e.getMessage());
        }
    }

    // Update transaction
    @PutMapping("/transactions/{id}")
    public Reply<Transaction> updateTransaction(@PathVariable Long id, @RequestBody Transaction transaction) {
        try {
            Transaction updatedTransaction = transactionService.updateTransaction(id, transaction);
            return Reply.success(updatedTransaction);
        } catch (RuntimeException e) {
            return Reply.notFound();
        } catch (Exception e) {
            return Reply.serverError("Failed to update transaction: " + e.getMessage());
        }
    }

    // Delete transaction
    @DeleteMapping("/transactions/{id}")
    public Reply<Void> deleteTransaction(@PathVariable Long id) {
        try {
            Optional<Transaction> transaction = transactionService.getTransactionById(id);
            if (transaction.isPresent()) {
                transactionService.deleteTransaction(id);
                return new Reply<>(0, "Transaction deleted successfully", null);
            } else {
                return Reply.notFound();
            }
        } catch (Exception e) {
            return Reply.serverError("Failed to delete transaction: " + e.getMessage());
        }
    }

    // Get total amount by customer
    @GetMapping("/transactions/customer/{customerId}/total")
    public Reply<Integer> getTotalAmountByCustomer(@PathVariable Integer customerId) {
        try {
            Integer total = transactionService.getTotalAmountByCustomer(customerId);
            return Reply.success(total);
        } catch (Exception e) {
            return Reply.serverError("Failed to calculate total amount: " + e.getMessage());
        }
    }
}