CREATE TABLE `bms`.`books_info` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(100) NOT NULL,
  `writer` VARCHAR(100) NOT NULL,
  `genre` VARCHAR(45) NULL,
  `release_date` DATE NULL,
  `pages` INT NULL,
  `company` VARCHAR(50) NULL,
  `price` DECIMAL NULL,
  PRIMARY KEY (`id`));