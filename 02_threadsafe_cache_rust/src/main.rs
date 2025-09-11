use std::collections::HashMap;
use std::hash::Hash;
use std::sync::{Arc, RwLock};
use std::thread;

pub struct Cache<K, V> {
    inner: RwLock<HashMap<K, V>>,
}

impl<K, V> Cache<K, V>
where
    K: Eq + Hash + Clone,
{
    pub fn new() -> Self {
        Self {
            inner: RwLock::new(HashMap::new()),
        }
    }

    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            inner: RwLock::new(HashMap::with_capacity(capacity)),
        }
    }

    pub fn get(&self, key: &K) -> Option<V>
    where
        V: Clone,
    {
        self.inner.read().ok()?.get(key).cloned()
    }

    pub fn put(&self, key: K, value: V) {
        if let Ok(mut map) = self.inner.write() {
            map.insert(key, value);
        }
    }

    pub fn get_or_load<F>(&self, key: K, loader: F) -> V
    where
        F: FnOnce(&K) -> V,
        V: Clone,
    {
        // First, try to get the value with a read lock (fast path)
        if let Ok(map) = self.inner.read() {
            if let Some(v) = map.get(&key) {
                return v.clone();
            }
        }
        
        // If not found, acquire write lock and check again (double-checked locking)
        if let Ok(mut map) = self.inner.write() {
            // Check again in case another thread inserted the value
            if let Some(v) = map.get(&key) {
                return v.clone();
            }
            
            // Load the value and insert it
            let v = loader(&key);
            map.insert(key, v.clone());
            return v;
        }
        
        // Fallback if lock acquisition fails
        loader(&key)
    }

    pub fn len(&self) -> usize {
        self.inner.read().map(|m| m.len()).unwrap_or(0)
    }

    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    pub fn contains_key(&self, key: &K) -> bool {
        self.inner.read().map(|m| m.contains_key(key)).unwrap_or(false)
    }

    pub fn remove(&self, key: &K) -> Option<V> {
        self.inner.write().ok()?.remove(key)
    }

    pub fn clear(&self) {
        if let Ok(mut map) = self.inner.write() {
            map.clear();
        }
    }
}

fn main() {
    // Create cache with initial capacity to avoid reallocations
    let cache = Arc::new(Cache::<i32, String>::with_capacity(16));

    println!("Starting concurrent cache operations...");
    let start = std::time::Instant::now();

    let handles: Vec<_> = (0..100)
        .map(|i| {
            let cache = Arc::clone(&cache);
            thread::spawn(move || {
                let key = i % 10; // More contention on fewer keys
                let value = cache.get_or_load(key, |k| {
                    // Simulate expensive computation
                    thread::sleep(std::time::Duration::from_millis(1));
                    format!("computed_value_{}", k * 100)
                });
                
                // Also test regular get operations
                if i % 3 == 0 {
                    cache.get(&key);
                }
                
                println!("Thread {}: key {} → value {}", i, key, value);
            })
        })
        .collect();

    for h in handles {
        h.join().unwrap();
    }

    let elapsed = start.elapsed();
    println!("\nCompleted in {:?}", elapsed);
    println!("Cache size: {}", cache.len());
    println!("Cache is empty: {}", cache.is_empty());
}
