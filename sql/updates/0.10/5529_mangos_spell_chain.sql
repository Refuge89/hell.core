DELETE FROM spell_chain WHERE spell_id IN (9787,9788,17039,17040,17041);
INSERT INTO spell_chain VALUES
(9787, 9785,2018,5),
(9788, 9785,2018,5),
(17039,9787,2018,6),
(17040,9787,2018,6),
(17041,9787,2018,6);

DELETE FROM spell_chain WHERE spell_id IN (28672,28675,28677);
INSERT INTO spell_chain VALUES
(28672,11611,2259,5),
(28675,11611,2259,5),
(28677,11611,2259,5);

DELETE FROM spell_chain WHERE spell_id IN (10656,10658,10660);
INSERT INTO spell_chain VALUES
(10656,10662,2108,5),
(10658,10662,2108,5),
(10660,10662,2108,5);

DELETE FROM spell_chain WHERE spell_id IN (20219,20222);
INSERT INTO spell_chain VALUES
(20219,12656,4036,5),
(20222,12656,4036,5);

DELETE FROM spell_chain WHERE spell_id IN (26797,26798,26801);
INSERT INTO spell_chain VALUES
(26797,12180,3908,5),
(26798,12180,3908,5),
(26801,12180,3908,5);
