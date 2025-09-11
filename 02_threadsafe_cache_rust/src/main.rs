use std::collections::HashMap;
use std::hash::Hash;
use std::sync::{Arc, Mutex};
use std::thread;

pub struct Cache<K, V> {
    inner: Mutex<HashMap<K, V>>,
}

impl<K, V> Cache<K, V>
where
    K: Eq + Hash + Clone,
{
    pub fn new() -> Self {
        Self {
            inner: Mutex::new(HashMap::new()),
        }
    }

    pub fn get(&self, key: &K) -> Option<V>
    where
        V: Clone,
    {
        self.inner.lock().ok()?.get(key).cloned()
    }

    pub fn put(&self, key: K, value: V) {
        if let Ok(mut map) = self.inner.lock() {
            map.insert(key, value);
        }
    }

    pub fn get_or_load<F>(&self, key: K, loader: F) -> V
    where
        F: FnOnce(&K) -> V,
        V: Clone,
    {
        if let Ok(mut map) = self.inner.lock() {
            if let Some(v) = map.get(&key) {
                return v.clone();
            }
            let v = loader(&key);
            map.insert(key.clone(), v.clone());
            return v;
        }
        loader(&key)
    }

    pub fn len(&self) -> usize {
        self.inner.lock().map(|m| m.len()).unwrap_or(0)
    }
}

fn main() {
    let cache = Arc::new(Cache::<i32, i32>::new());

    let handles: Vec<_> = (0..10)
        .map(|i| {
            let cache = Arc::clone(&cache);
            thread::spawn(move || {
                let key = i % 3;
                let value = cache.get_or_load(key, |k| k * 10);
                println!("key {key} → value {value}");
            })
        })
        .collect();

    for h in handles {
        h.join().unwrap();
    }

    println!("Cache size: {}", cache.len());
}
