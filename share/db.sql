DROP TABLE IF EXISTS `tcs_app_user`;
CREATE TABLE `tcs_app_user` (
  `name` varchar(20) NOT NULL,
  `pswd` varchar(20) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tcs_app_room`;
CREATE TABLE `tcs_app_room` (
  `id` int(11) NOT NULL,
  `settemp` decimal(10,4) NOT NULL,
  `temp` decimal(10,4) NOT NULL,
  `wdspd` int(11) NOT NULL,
  `setwdspd` int(11) NOT NULL,
  `token` varchar(10) DEFAULT NULL,
  `state` int(11) NOT NULL,
  `mode` tinyint(11) NOT NULL,
  `costs` decimal(10,4) NOT NULL,
  `power` decimal(10,4) NOT NULL,
  `duration` bigint(20) NOT NULL,
  `user_id_id` varchar(20) DEFAULT NULL,
  `start` datetime(6) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `tcs_app_room_user_id_id_700672d0_fk_tcs_app_user_name` (`user_id_id`),
  CONSTRAINT `tcs_app_room_user_id_id_700672d0_fk_tcs_app_user_name` FOREIGN KEY (`user_id_id`) REFERENCES `tcs_app_user` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tcs_app_host`;
CREATE TABLE `tcs_app_host` (
  `id` int(11) NOT NULL,
  `start_temp` int(11) NOT NULL,
  `mode` tinyint(11) NOT NULL,
  `para_low` decimal(10,4) NOT NULL,
  `para_mid` decimal(10,4) NOT NULL,
  `para_high` decimal(10,4) NOT NULL,
  `low_rate` decimal(10,4) NOT NULL,
  `middle_rate` decimal(10,4) NOT NULL,
  `high_rate` decimal(10,4) NOT NULL,
  `max_temp` int(11) NOT NULL,
  `min_temp` int(11) NOT NULL,
  `default_temp` int(11) NOT NULL,
  `mode_sche` int(11) NOT NULL,
  `state` tinyint(1) NOT NULL,
  `default_wdspd` int(11) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tcs_app_manager`;
CREATE TABLE `tcs_app_manager` (
  `name` varchar(20) NOT NULL,
  `pswd` varchar(20) NOT NULL,
  `privilege` int(11) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `tcs_app_bill`;
CREATE TABLE `tcs_app_bill` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `start` datetime(6) NOT NULL,
  `duration` bigint(20) NOT NULL,
  `costs` decimal(10,4) NOT NULL,
  `wdspd` int(11) NOT NULL,
  `start_temp` decimal(10,4) DEFAULT NULL,
  `end_temp` decimal(10,4) DEFAULT NULL,
  `rate` decimal(10,4) DEFAULT NULL,
  `action` int(11) DEFAULT '0',
  `room_id_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `tcs_app_bill_room_id_id_31f98b57_fk_tcs_app_room_id` (`room_id_id`),
  CONSTRAINT `tcs_app_bill_room_id_id_31f98b57_fk_tcs_app_room_id` FOREIGN KEY (`room_id_id`) REFERENCES `tcs_app_room` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3740 DEFAULT CHARSET=latin1;


DROP TABLE IF EXISTS `tcs_app_request`;
CREATE TABLE `tcs_app_request` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `settemp` decimal(10,4) NOT NULL,
  `setwdspd` int(11) NOT NULL,
  `state` int(11) NOT NULL,
  `user_id_id` varchar(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `tcs_request_user_id_id_6ece52f5_fk_tcs_user_name` (`user_id_id`),
  CONSTRAINT `tcs_request_user_id_id_6ece52f5_fk_tcs_user_name` FOREIGN KEY (`user_id_id`) REFERENCES `tcs_app_user` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=936 DEFAULT CHARSET=latin1;

