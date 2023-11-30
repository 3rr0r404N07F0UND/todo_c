CREATE TABLE todo_table(
    id int NOT NULL PRIMARY KEY AUTO_INCREMENT COMMENT 'ID',
    create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create Time',
    title VARCHAR(255) COMMENT 'Title',
    contents TEXT COMMENT 'Contents'
);