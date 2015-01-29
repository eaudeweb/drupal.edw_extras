<?php

class EdwDatabase {
	/**
	 * Function to retrive the size of a MySQL varchar column from a table.
	 *
	 * @param string $table
	 *   Target database table name
	 * @param string $column
	 *   Target column
	 *
	 * @return int
	 *   Column size in characters
	 */
	public function get_column_length_mysql($table, $column) {
		return db_query("
			SELECT CHARACTER_MAXIMUM_LENGTH
			FROM information_schema.columns
			WHERE table_schema = DATABASE()
				AND table_name = :table AND COLUMN_NAME = :column",
			array(':table' => $table, ':column' => $column)
		)->fetchField(0);
	}
}
