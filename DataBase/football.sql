-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Nov 25, 2021 at 01:36 PM
-- Server version: 10.4.21-MariaDB
-- PHP Version: 8.0.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `football`
--

-- --------------------------------------------------------

--
-- Table structure for table `clan`
--

CREATE TABLE `clan` (
  `ClanID` int(11) UNSIGNED NOT NULL,
  `Logo` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `Trophies` mediumint(11) UNSIGNED NOT NULL DEFAULT 0,
  `Type` tinyint(3) UNSIGNED NOT NULL DEFAULT 0,
  `Requirement` smallint(5) UNSIGNED NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `clan`
--

INSERT INTO `clan` (`ClanID`, `Logo`, `Trophies`, `Type`, `Requirement`) VALUES
(5, 0, 0, 0, 0),
(6, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `match_info`
--

CREATE TABLE `match_info` (
  `id_mt` int(11) NOT NULL,
  `red_team` int(11) NOT NULL,
  `blue_team` int(11) NOT NULL,
  `Status` int(1) NOT NULL,
  `Created` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `match_info`
--

INSERT INTO `match_info` (`id_mt`, `red_team`, `blue_team`, `Status`, `Created`) VALUES
(1, 1, 2, 0, '2021-05-08 23:55:25'),
(2, 3, 4, 0, '2021-05-09 00:00:41'),
(3, 7, 8, 0, '2021-08-01 11:05:13'),
(4, 9, 10, 0, '2021-08-01 11:29:19'),
(5, 11, 12, 0, '2021-08-01 12:01:00'),
(6, 12, 13, 0, '2021-08-01 14:31:10'),
(7, 13, 14, 0, '2021-08-01 14:36:31');

-- --------------------------------------------------------

--
-- Table structure for table `match_making`
--

CREATE TABLE `match_making` (
  `Id` int(11) NOT NULL,
  `Team_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `match_making`
--

INSERT INTO `match_making` (`Id`, `Team_id`) VALUES
(49, 14);

-- --------------------------------------------------------

--
-- Table structure for table `players`
--

CREATE TABLE `players` (
  `PlayerID` int(7) UNSIGNED NOT NULL,
  `Name` varchar(14) NOT NULL,
  `Status` tinyint(3) UNSIGNED NOT NULL DEFAULT 1,
  `Flag` tinyint(1) UNSIGNED NOT NULL DEFAULT 0,
  `Gem` mediumint(7) UNSIGNED NOT NULL DEFAULT 0,
  `Gold` mediumint(7) UNSIGNED NOT NULL DEFAULT 0,
  `PlayerChar` tinyint(2) UNSIGNED NOT NULL DEFAULT 0,
  `Stars` tinyint(3) UNSIGNED NOT NULL DEFAULT 0,
  `Trophies` mediumint(7) UNSIGNED NOT NULL DEFAULT 0,
  `Energy` tinyint(1) UNSIGNED NOT NULL DEFAULT 5,
  `Pass` tinyint(1) UNSIGNED NOT NULL DEFAULT 0,
  `ClanID` int(11) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `players`
--

INSERT INTO `players` (`PlayerID`, `Name`, `Status`, `Flag`, `Gem`, `Gold`, `PlayerChar`, `Stars`, `Trophies`, `Energy`, `Pass`, `ClanID`) VALUES
(15, '', 1, 0, 0, 0, 0, 0, 0, 5, 0, 6),
(18, '12', 1, 0, 0, 0, 0, 0, 0, 5, 0, 5),
(19, '512', 1, 0, 0, 0, 0, 0, 0, 5, 0, 6),
(20, '523', 1, 0, 0, 0, 0, 0, 0, 5, 0, 6),
(21, '142', 1, 0, 0, 0, 0, 0, 0, 5, 0, 6);

-- --------------------------------------------------------

--
-- Table structure for table `player_info`
--

CREATE TABLE `player_info` (
  `ID` int(7) UNSIGNED NOT NULL,
  `Win` int(11) UNSIGNED NOT NULL,
  `Draw` int(11) UNSIGNED NOT NULL,
  `Lose` int(11) UNSIGNED NOT NULL,
  `Goals` smallint(11) UNSIGNED NOT NULL,
  `Assists` smallint(11) UNSIGNED NOT NULL,
  `Passes` smallint(11) UNSIGNED NOT NULL,
  `MVP` smallint(5) UNSIGNED NOT NULL DEFAULT 0,
  `WinStreak` smallint(10) UNSIGNED NOT NULL DEFAULT 0,
  `BestStreak` smallint(10) UNSIGNED NOT NULL DEFAULT 0,
  `Tackles` smallint(10) UNSIGNED NOT NULL DEFAULT 0,
  `Tackled` smallint(10) UNSIGNED NOT NULL DEFAULT 0,
  `Rank` mediumint(10) UNSIGNED NOT NULL DEFAULT 0,
  `PlayerID` int(7) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `player_info`
--

INSERT INTO `player_info` (`ID`, `Win`, `Draw`, `Lose`, `Goals`, `Assists`, `Passes`, `MVP`, `WinStreak`, `BestStreak`, `Tackles`, `Tackled`, `Rank`, `PlayerID`) VALUES
(7, 10, 2, 5, 12, 4, 15, 3, 2, 5, 18, 10, 2000, 21);

-- --------------------------------------------------------

--
-- Table structure for table `tblmember`
--

CREATE TABLE `tblmember` (
  `id` int(11) NOT NULL,
  `pl_id` varchar(50) NOT NULL,
  `fName` varchar(30) NOT NULL,
  `lName` varchar(30) NOT NULL,
  `email` varchar(50) NOT NULL,
  `steam_id` int(11) NOT NULL,
  `password` varchar(60) NOT NULL,
  `birthdate` text NOT NULL,
  `gender` varchar(20) NOT NULL,
  `player_type` int(2) NOT NULL,
  `Status` int(2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tblmember`
--

INSERT INTO `tblmember` (`id`, `pl_id`, `fName`, `lName`, `email`, `steam_id`, `password`, `birthdate`, `gender`, `player_type`, `Status`) VALUES
(12, 'pl_5fdfa4b645303', 'Team2pl_1', '', '', 0, '', '', '', 0, 3),
(13, 'pl_5fdfa4b8e1502', 'Team2pl_2', '', '', 0, '', '', '', 0, 3),
(14, 'pl_5fdfa4bb37815', 'Team2pl_3', '', '', 0, '', '', '', 0, 3),
(16, 'pl_5ff0b2bd910e6', 'mousa', '', '', 2147483647, '', '', '', 0, 3),
(19, 'pl_5ff9a04d6a501', 'Elie', '', '', 2147483647, '', '', '', 0, 3),
(20, 'pl_603fa37ec34b1', 'AhmedTest', '', '', 2147483647, '', '', '', 0, 3),
(22, 'pl_6095b87c0503a', 'ss', '', '', 0, '', '', '', 0, 0),
(23, 'pl_6095b8f6a3fb2', 'aa', '', '', 0, '', '', '', 0, 0),
(24, 'pl_6095bb2899517', 'ss', '', 'ss@dff.com', 0, '', '', '', 0, 0),
(25, 'pl_6095cb5f4a81b', 'dd', '', 'dd@ddd', 0, '', '', '', 0, 0),
(26, 'pl_6095cb6ab40b8', 'ss', '', 'ss@gg', 0, '', '', '', 0, 0),
(27, 'pl_609725a308a0c', 'Ahmed', '', 'ahmed@gh.com', 0, '', '', '', 0, 0),
(28, 'pl_6105c29cc30ee', 'ahmef', '', 'aa@ff.com', 0, '', '', '', 0, 0),
(29, 'pl_6106b1327bc8d', 'Ahmed52', '', 'ag@hh.con', 0, '', '', '', 0, 0),
(30, 'pl_6106b148add0b', 'Ali82', '', 'ali82@gg.cokm', 0, '', '', '', 0, 0),
(31, 'pl_6155b5056bdf2', 'ww', '', 'ddd@ff.com', 0, '', '', '', 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `team_info`
--

CREATE TABLE `team_info` (
  `team_id` int(11) NOT NULL,
  `Owner_id` varchar(20) NOT NULL,
  `Secret_key` varchar(20) NOT NULL,
  `Created` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `team_info`
--

INSERT INTO `team_info` (`team_id`, `Owner_id`, `Secret_key`, `Created`) VALUES
(1, 'pl_6095cb6ab40b8', 'sk_609724b050743', '2021-05-08 23:54:24'),
(2, 'pl_6095cb6ab40b8', 'sk_609724d201c55', '2021-05-08 23:54:58'),
(3, 'pl_609725a308a0c', 'sk_609725ced3dd3', '2021-05-08 23:59:10'),
(4, 'pl_609725a308a0c', 'sk_609725d4bda58', '2021-05-08 23:59:16'),
(5, 'pl_6105c29cc30ee', 'sk_6105c2a09cecb', '2021-07-31 21:37:36'),
(6, 'pl_6105c29cc30ee', 'sk_610676574a919', '2021-08-01 10:24:23'),
(7, 'pl_6105c29cc30ee', 'sk_61067e4d0cd1d', '2021-08-01 10:58:21'),
(8, 'pl_6105c29cc30ee', 'sk_61067fd81d291', '2021-08-01 11:04:56'),
(9, 'pl_6105c29cc30ee', 'sk_6106852882302', '2021-08-01 11:27:36'),
(10, 'pl_6105c29cc30ee', 'sk_61068571d339c', '2021-08-01 11:28:49'),
(11, 'pl_6105c29cc30ee', 'sk_6106880fb4f19', '2021-08-01 11:39:59'),
(12, 'pl_6105c29cc30ee', 'sk_61068cdc80006', '2021-08-01 12:00:28'),
(13, 'pl_6105c29cc30ee', 'sk_6106b01a7d84e', '2021-08-01 14:30:50'),
(14, 'pl_6106b148add0b', 'sk_6106b14b761d3', '2021-08-01 14:35:55');

-- --------------------------------------------------------

--
-- Table structure for table `team_members`
--

CREATE TABLE `team_members` (
  `id_tm` int(11) NOT NULL,
  `is_owner` int(1) NOT NULL,
  `pl_id` varchar(50) NOT NULL,
  `team_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `team_members`
--

INSERT INTO `team_members` (`id_tm`, `is_owner`, `pl_id`, `team_id`) VALUES
(2, 0, 'p1jjhja', 10),
(3, 0, 'p1jjhja', 10),
(4, 0, 'pl_6095cb6ab40b8', 15),
(5, 0, 'pl_6095cb6ab40b8', 18),
(6, 0, 'pl_6095cb6ab40b8', 18),
(7, 0, 'pl_6095cb6ab40b8', 19),
(8, 0, 'pl_6095cb6ab40b8', 19),
(9, 0, 'pl_6095cb6ab40b8', 20),
(10, 0, 'pl_6095cb6ab40b8', 20),
(11, 0, 'pl_6095cb6ab40b8', 21),
(12, 0, 'pl_6095cb6ab40b8', 21),
(13, 0, 'pl_6095cb6ab40b8', 22),
(14, 0, 'pl_6095cb6ab40b8', 22),
(15, 0, 'pl_6095cb6ab40b8', 23),
(16, 0, 'pl_6095cb6ab40b8', 23),
(17, 0, 'pl_6095cb6ab40b8', 24),
(18, 0, 'pl_6095cb6ab40b8', 24),
(19, 0, 'pl_6095cb6ab40b8', 26),
(20, 0, 'pl_6095cb6ab40b8', 26),
(21, 0, 'pl_6095cb6ab40b8', 27),
(22, 0, 'pl_6095cb6ab40b8', 27),
(23, 0, 'pl_6095cb6ab40b8', 28),
(24, 0, 'pl_6095cb6ab40b8', 28),
(25, 0, 'pl_6095cb6ab40b8', 29),
(26, 0, 'pl_6095cb6ab40b8', 29),
(27, 0, 'pl_6095cb6ab40b8', 30),
(28, 0, 'pl_6095cb6ab40b8', 30),
(29, 0, 'pl_6095cb6ab40b8', 31),
(30, 0, 'pl_6095cb6ab40b8', 31),
(31, 0, 'pl_6095cb6ab40b8', 33),
(32, 0, 'pl_6095cb6ab40b8', 33),
(33, 0, 'pl_6095cb6ab40b8', 34),
(34, 0, 'pl_6095cb6ab40b8', 34),
(35, 0, 'pl_6095cb6ab40b8', 35),
(36, 0, 'pl_6095cb6ab40b8', 35),
(37, 0, 'pl_6095cb6ab40b8', 1),
(38, 0, 'pl_6095cb6ab40b8', 1),
(39, 0, 'pl_6095cb6ab40b8', 2),
(40, 0, 'pl_6095cb6ab40b8', 2),
(41, 0, 'pl_609725a308a0c', 3),
(42, 0, 'pl_609725a308a0c', 3),
(43, 0, 'pl_609725a308a0c', 4),
(44, 0, 'pl_609725a308a0c', 4),
(45, 0, 'pl_6105c29cc30ee', 6),
(46, 0, 'pl_6105c29cc30ee', 7),
(47, 0, 'pl_6105c29cc30ee', 8),
(48, 0, 'pl_6105c29cc30ee', 9),
(49, 0, 'pl_6105c29cc30ee', 10),
(50, 0, 'pl_6105c29cc30ee', 11),
(51, 0, 'pl_6105c29cc30ee', 12),
(52, 0, 'pl_6105c29cc30ee', 13),
(53, 0, 'pl_6106b1327bc8d', 14);

-- --------------------------------------------------------

--
-- Table structure for table `url_service`
--

CREATE TABLE `url_service` (
  `id` int(11) NOT NULL,
  `path` text NOT NULL,
  `Status` int(11) NOT NULL,
  `match_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `url_service`
--

INSERT INTO `url_service` (`id`, `path`, `Status`, `match_id`) VALUES
(1, '127.0.0.1:3000', 0, 0),
(2, '127.0.0.1:3001', 1, 0),
(3, '127.0.0.1:3002', 1, 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `clan`
--
ALTER TABLE `clan`
  ADD PRIMARY KEY (`ClanID`) USING BTREE;

--
-- Indexes for table `match_info`
--
ALTER TABLE `match_info`
  ADD PRIMARY KEY (`id_mt`);

--
-- Indexes for table `match_making`
--
ALTER TABLE `match_making`
  ADD PRIMARY KEY (`Id`);

--
-- Indexes for table `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`PlayerID`),
  ADD UNIQUE KEY `Name` (`Name`),
  ADD KEY `PlayerRef` (`ClanID`);

--
-- Indexes for table `player_info`
--
ALTER TABLE `player_info`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `PlayerID` (`PlayerID`);

--
-- Indexes for table `tblmember`
--
ALTER TABLE `tblmember`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `team_info`
--
ALTER TABLE `team_info`
  ADD PRIMARY KEY (`team_id`);

--
-- Indexes for table `team_members`
--
ALTER TABLE `team_members`
  ADD PRIMARY KEY (`id_tm`);

--
-- Indexes for table `url_service`
--
ALTER TABLE `url_service`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `clan`
--
ALTER TABLE `clan`
  MODIFY `ClanID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `match_info`
--
ALTER TABLE `match_info`
  MODIFY `id_mt` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT for table `match_making`
--
ALTER TABLE `match_making`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=50;

--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `PlayerID` int(7) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;

--
-- AUTO_INCREMENT for table `player_info`
--
ALTER TABLE `player_info`
  MODIFY `ID` int(7) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT for table `tblmember`
--
ALTER TABLE `tblmember`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=32;

--
-- AUTO_INCREMENT for table `team_info`
--
ALTER TABLE `team_info`
  MODIFY `team_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=15;

--
-- AUTO_INCREMENT for table `team_members`
--
ALTER TABLE `team_members`
  MODIFY `id_tm` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=54;

--
-- AUTO_INCREMENT for table `url_service`
--
ALTER TABLE `url_service`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `players`
--
ALTER TABLE `players`
  ADD CONSTRAINT `PlayerRef` FOREIGN KEY (`ClanID`) REFERENCES `clan` (`ClanID`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `player_info`
--
ALTER TABLE `player_info`
  ADD CONSTRAINT `player_info_ibfk_1` FOREIGN KEY (`PlayerID`) REFERENCES `players` (`PlayerID`) ON UPDATE NO ACTION;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
