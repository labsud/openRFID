-- Adminer 4.6.2 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

DROP DATABASE IF EXISTS `labsud`;
CREATE DATABASE `labsud` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `labsud`;

DROP TABLE IF EXISTS `qualifications`;
CREATE TABLE `qualifications` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `tag` tinytext NOT NULL,
  `password` tinytext NOT NULL,
  `laser` tinyint(4) NOT NULL DEFAULT '0',
  `door` tinyint(4) NOT NULL DEFAULT '0',
  `nom` tinytext NOT NULL,
  `prenom` tinytext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `qualifications` (`id`, `user_id`, `tag`, `password`, `laser`, `door`, `nom`, `prenom`) VALUES
(1,	1337,	'2119613520',	'1111',	1,	0,	'MOLIA',	'Lucas'),
(2,	42,	'2119517424',	'159753',	0,	0,	'HONYME',	'Anne'),
(3,	0,	'2896416342',	'0000',	0,	1,	'VADOR',	'Dark');

-- 2018-07-13 10:18:36
