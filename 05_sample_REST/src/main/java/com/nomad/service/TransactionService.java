package com.nomad.service;

import com.nomad.entity.Transaction;
import com.nomad.repository.TransactionRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@Service
@Transactional
public class TransactionService {

    @Autowired
    private TransactionRepository transactionRepository;

    public List<Transaction> getAllTransactions() {
        return transactionRepository.findAll();
    }

    public Optional<Transaction> getTransactionById(Long id) {
        return transactionRepository.findById(id);
    }

    public List<Transaction> getTransactionsByCustomerId(Integer customerId) {
        return transactionRepository.findByCustomerId(customerId);
    }

    public List<Transaction> getValidTransactions() {
        return transactionRepository.findByIsValidTrue();
    }

    public List<Transaction> searchByCustomerName(String customerName) {
        return transactionRepository.findByCustomerNameContainingIgnoreCase(customerName);
    }

    public List<Transaction> getTransactionsByAmountRange(Integer minAmount, Integer maxAmount) {
        return transactionRepository.findByAmountBetween(minAmount, maxAmount);
    }

    public Transaction saveTransaction(Transaction transaction) {
        // Check if this is a new transaction (no ID set)
        boolean isNewTransaction = transaction.getPkId() == null;
        
        if (isNewTransaction) {
            // For new transactions: set create_time if not provided, and default is_valid
            if (transaction.getCreateTime() == null) {
                transaction.setCreateTime(LocalDateTime.now());
            }
            if (transaction.getIsValid() == null) {
                transaction.setIsValid(true);
            }
        } else {
            // For updates: Never change create_time, only update is_valid if explicitly provided
            Optional<Transaction> existing = transactionRepository.findById(transaction.getPkId());
            if (existing.isPresent()) {
                Transaction existingTransaction = existing.get();
                // Always preserve create_time from existing record
                transaction.setCreateTime(existingTransaction.getCreateTime());
                
                // Only update is_valid if it was explicitly provided (not null)
                if (transaction.getIsValid() == null) {
                    transaction.setIsValid(existingTransaction.getIsValid());
                }
            }
        }
        
        return transactionRepository.save(transaction);
    }

    public Transaction createTransaction(Transaction transaction) {
        // For new transactions, always set create_time and default is_valid
        transaction.setPkId(null); // Ensure it's treated as new
        transaction.setCreateTime(LocalDateTime.now());
        if (transaction.getIsValid() == null) {
            transaction.setIsValid(true);
        }
        return transactionRepository.save(transaction);
    }

    public Transaction updateTransaction(Long id, Transaction updatedTransaction) {
        return transactionRepository.findById(id)
                .map(existingTransaction -> {
                    // Update only non-null fields, but never update create_time
                    if (updatedTransaction.getCustomerId() != null) {
                        existingTransaction.setCustomerId(updatedTransaction.getCustomerId());
                    }
                    if (updatedTransaction.getCustomerName() != null) {
                        existingTransaction.setCustomerName(updatedTransaction.getCustomerName());
                    }
                    if (updatedTransaction.getAmount() != null) {
                        existingTransaction.setAmount(updatedTransaction.getAmount());
                    }
                    // Never update create_time - it's set only once during insert
                    
                    // Only update is_valid if explicitly provided (not null)
                    if (updatedTransaction.getIsValid() != null) {
                        existingTransaction.setIsValid(updatedTransaction.getIsValid());
                    }
                    
                    return transactionRepository.save(existingTransaction);
                })
                .orElseThrow(() -> new RuntimeException("Transaction not found with id: " + id));
    }

    public void deleteTransaction(Long id) {
        transactionRepository.deleteById(id);
    }

    public Integer getTotalAmountByCustomer(Integer customerId) {
        Integer total = transactionRepository.getTotalAmountByCustomer(customerId);
        return total != null ? total : 0;
    }
}