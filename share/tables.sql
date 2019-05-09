DROP TABLE IF EXISTS usr;
CREATE TABLE usr (
  name varchar(20) NOT NULL,
  pswd varchar(20) NOT NULL,
  PRIMARY KEY (name)
);

DROP TABLE IF EXISTS room;
CREATE TABLE room (
);

DROP TABLE IF EXISTS request;
CREATE TABLE request (
);

DROP TABLE IF EXISTS billing;
CREATE TABLE billing (
);
