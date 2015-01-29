<?php

class EdwCacheDomain {

	static function cache_get($cache_domain, $cache_key, $callback) {
		$ret = NULL;
		$cache_full_key = $cache_domain . '.' . $cache_key;
		$cache = cache_get($cache_full_key);
		if(!isset($cache)) {
			$data = $callback();
			if ($data) {
				cache_set($cache_full_key, $data);
			}
			return $data;
		} else {
			$ret =$cache->data;
		}
		return $ret;
	}

	static function cache_get_array($cache_domain, $cache_key, $callback, $arg) {
		$ret = NULL;
		$cache_full_key = $cache_domain . '.' . $cache_key;
		$cache = cache_get($cache_full_key);
		if(!isset($cache->data[$arg])) {
			$data = isset($cache->data) ? $cache->data : array();
			$ret = $data[$arg] = $callback($arg);
			cache_set($cache_full_key, $data);
		} else {
			$ret =$cache->data[$arg];
		}
		return $ret;
	}
}
