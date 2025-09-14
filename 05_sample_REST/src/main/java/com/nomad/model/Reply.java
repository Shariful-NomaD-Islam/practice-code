package com.nomad.model;

import java.io.Serializable;

public class Reply<T> implements Serializable {
    private int code;
    private String message;
    private T payload;
    
    // Default constructor
    public Reply() {
        this.code = 0;
        this.message = "OK";
    }
    
    // Constructor for success response
    public Reply(T payload) {
        this.code = 0;
        this.message = "OK";
        this.payload = payload;
    }
    
    // Constructor for custom response
    public Reply(int code, String message, T payload) {
        this.code = code;
        this.message = message;
        this.payload = payload;
    }
    
    // Constructor for error response
    public Reply(int code, String message) {
        this.code = code;
        this.message = message;
        this.payload = null;
    }
    
    // Static factory methods for common responses
    public static <T> Reply<T> success(T payload) {
        return new Reply<>(payload);
    }
    
    public static <T> Reply<T> success() {
        return new Reply<>();
    }
    
    public static <T> Reply<T> error(int code, String message) {
        return new Reply<>(code, message);
    }
    
    public static <T> Reply<T> notFound() {
        return new Reply<>(404, "Resource not found");
    }
    
    public static <T> Reply<T> badRequest(String message) {
        return new Reply<>(400, message);
    }
    
    public static <T> Reply<T> serverError(String message) {
        return new Reply<>(500, message);
    }
    
    // Getters and Setters
    public int getCode() {
        return code;
    }
    
    public void setCode(int code) {
        this.code = code;
    }
    
    public String getMessage() {
        return message;
    }
    
    public void setMessage(String message) {
        this.message = message;
    }
    
    public T getPayload() {
        return payload;
    }
    
    public void setPayload(T payload) {
        this.payload = payload;
    }
    
    @Override
    public String toString() {
        return "Reply{" +
                "code=" + code +
                ", message='" + message + '\'' +
                ", payload=" + payload +
                '}';
    }
}