#ifndef SQLITE_SCHEMA_H_INCLUDED
#define SQLITE_SCHEMA_H_INCLUDED

#define COUNTY_SCHEMA "CREATE TABLE IF NOT EXISTS county(\n\
    county_id INT PRIMARY KEY,\n\
    county_desc VARCHAR(15)\n\
);"

#define VOTER_SCHEMA "CREATE TABLE IF NOT EXISTS voter(\n\
    ncid CHAR(12) PRIMARY KEY,\n\
    last_name VARCHAR(25),\n\
    first_name VARCHAR(20),\n\
    middle_name VARCHAR(20),\n\
    name_suffix_lbl CHAR(3)\n\
);"

#define STATUS_SCHEMA "CREATE TABLE IF NOT EXISTS status(\n\
    status_cd CHAR(2) PRIMARY KEY,\n\
    voter_status_desc VARCHAR(25)\n\
);"
#define REASON_SCHEMA "CREATE TABLE IF NOT EXISTS reason(\n\
    reason_cd VARCHAR(2) PRIMARY KEY,\n\
    voter_status_reason_desc VARCHAR(60)\n\
);"
#define REGISTRATION_SCHEMA "CREATE TABLE IF NOT EXISTS registration(\n\
    county_id INT,\n\
    voter_reg_num CHAR(12),\n\
    ncid CHAR(12),\n\
    status_cd CHAR(2),\n\
    reason_cd VARCHAR(2),\n\
    res_street_address VARCHAR(65),\n\
    res_city_desc VARCHAR(60),\n\
    state_cd VARCHAR(2),\n\
    zip_code CHAR(9),\n\
    mail_addr1 varchar(40),\n\
    mail_addr2 VARCHAR(40),\n\
    mail_addr3 VARCHAR(40),\n\
    mail_addr4 VARCHAR(40),\n\
    mail_city VARCHAR(30),\n\
    mail_state VARCHAR(2),\n\
    mail_zipcode CHAR(9),\n\
    full_phone_number VARCHAR(12),\n\
    confidential_ind CHAR(1),\n\
    registr_dt CHAR(10),\n\
    race_code CHAR(3),\n\
    ethnic_code CHAR(3),\n\
    party_cd CHAR(3),\n\
    gender_code CHAR(1),\n\
    birth_year CHAR(4),\n\
    age_at_year_end CHAR(3),\n\
    birth_state VARCHAR(2),\n\
    drivers_lic CHAR(1),\n\
    precinct_abbrv VARCHAR(6),\n\
    municipality_abbrv VARCHAR(6),\n\
    ward_abbrv VARCHAR(6),\n\
    cong_dist_abbrv VARCHAR(6),\n\
    super_court_abbrv VARCHAR(6),\n\
    judic_dist_abbrv VARCHAR(6),\n\
    nc_senate_abbrv VARCHAR(6),\n\
    nc_house_abbrv VARCHAR(6),\n\
    county_commiss_abbrv VARCHAR(6),\n\
    township_abbrv VARCHAR(6),\n\
    school_dist_abbrv VARCHAR(6),\n\
    fire_dist_abbrv VARCHAR(6),\n\
    water_dist_abbrv VARCHAR(6),\n\
    sewer_dist_abbrv VARCHAR(6),\n\
    sanit_dist_abbrv VARCHAR(6),\n\
    rescue_dist_abbrv VARCHAR(6),\n\
    munic_dist_abbrv VARCHAR(6),\n\
    dist_1_abbrv VARCHAR(6),\n\
    vtd_abbrv VARCHAR(6),\n\
    PRIMARY KEY(county_id, voter_reg_num),\n\
    FOREIGN KEY(status_cd) REFERENCES status(status_cd),\n\
    FOREIGN KEY(ncid) REFERENCES voter(ncid),\n\
    FOREIGN KEY(reason_cd) REFERENCES reason(reason_cd),\n\
    FOREIGN KEY(county_id) REFERENCES county(county_id)\n\
);"
#endif
