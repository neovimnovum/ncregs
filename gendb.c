#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <sqlite3.h>
#include "nc_data.h"
#include "sqlite_schema.h"

sqlite3 *main_db = NULL;

static char *main_column_table[67] = {
    "county_id", "county_desc", "voter_reg_num", "ncid", "last_name", "first_name",
    "middle_name", "name_suffix_lbl", "status_cd", "voter_status_desc", "reason_cd",
    "voter_status_reason_desc", "res_street_address", "res_city_desc", "state_cd",
    "zip_code", "mail_addr1", "mail_addr2", "mail_addr3", "mail_addr4", "mail_city",
    "mail_state", "mail_zipcode", "full_phone_number", "confidential_ind", "registr_dt",
    "race_code", "ethnic_code", "party_cd", "gender_code", "birth_year", "age_at_year_end",
    "birth_state", "drivers_lic", "precinct_abbrv", "precinct_desc", "municipality_abbrv",
    "municipality_desc", "ward_abbrv", "ward_desc", "cong_dist_abbrv", "super_court_abbrv",
    "judic_dist_abbrv", "nc_senate_abbrv", "nc_house_abbrv", "county_commiss_abbrv",
    "county_commiss_desc", "township_abbrv", "township_desc", "school_dist_abbrv",
    "school_dist_desc", "fire_dist_abbrv", "fire_dist_desc", "water_dist_abbrv",
    "water_dist_desc", "sewer_dist_abbrv", "sewer_dist_desc", "sanit_dist_abbrv",
    "sanit_dist_desc", "rescue_dist_abbrv", "rescue_dist_desc", "munic_dist_abbrv",
    "munic_dist_desc", "dist_1_abbrv", "dist_1_desc", "vtd_abbrv", "vtd_desc"
}; 

const char *sqlite_begin_transaction = "BEGIN TRANSACTION;";
const char *sqlite_update_county = "INSERT INTO county (county_id, county_desc) "
    "VALUES (:county_id, :county_desc)"
    "ON CONFLICT (county_id) DO NOTHING;";
const char *sqlite_update_voter = "INSERT INTO voter (ncid, last_name, first_name, "
    "middle_name, name_suffix_lbl)"
    "VALUES (:ncid, :last_name, :first_name, :middle_name, :name_suffix_lbl)"
    "ON CONFLICT (ncid) DO NOTHING;";
const char *sqlite_update_status = "INSERT INTO status (status_cd, voter_status_desc) VALUES (:status_cd, :voter_status_desc)"
    "ON CONFLICT (status_cd) DO NOTHING;";
const char *sqlite_update_reason = "INSERT INTO reason (reason_cd, voter_status_reason_desc) "
    "VALUES (:reason_cd, :voter_status_reason_desc) ON CONFLICT (reason_cd) DO NOTHING;";
const char *sqlite_update_registration = "INSERT INTO registration "
    "(county_id, voter_reg_num, ncid, status_cd, reason_cd, "
    "res_street_address, res_city_desc, state_cd, zip_code, mail_addr1, mail_addr2, "
    "mail_addr3, mail_addr4, mail_city, mail_state, mail_zipcode, full_phone_number, "
    "confidential_ind, registr_dt, race_code, ethnic_code, party_cd, gender_code, birth_year, "
    "age_at_year_end, birth_state, drivers_lic, precinct_abbrv, municipality_abbrv, ward_abbrv, "
    "cong_dist_abbrv, super_court_abbrv, judic_dist_abbrv, nc_senate_abbrv, nc_house_abbrv, "
    "county_commiss_abbrv, township_abbrv, school_dist_abbrv, fire_dist_abbrv, "
    "water_dist_abbrv, sewer_dist_abbrv, sanit_dist_abbrv, rescue_dist_abbrv, "
    "munic_dist_abbrv, dist_1_abbrv, vtd_abbrv)"
    "VALUES (:county_id, :voter_reg_num, :ncid, :status_cd, :reason_cd, "
    ":res_street_address, :res_city_desc, :state_cd, :zip_code, :mail_addr1, :mail_addr2, "
    ":mail_addr3, :mail_addr4, :mail_city, :mail_state, :mail_zipcode, :full_phone_number, "
    ":confidential_ind, :registr_dt, :race_code, :ethnic_code, :party_cd, :gender_code, "
    ":birth_year, :age_at_year_end, :birth_state, :drivers_lic, :precinct_abbrv, "
    ":municipality_abbrv, :ward_abbrv, :cong_dist_abbrv, :super_court_abbrv, "
    ":judic_dist_abbrv, :nc_senate_abbrv, :nc_house_abbrv, :county_commiss_abbrv, "
    ":township_abbrv, :school_dist_abbrv, :fire_dist_abbrv, :water_dist_abbrv, "
    ":sewer_dist_abbrv, :sanit_dist_abbrv, :rescue_dist_abbrv, :munic_dist_abbrv, "
    ":dist_1_abbrv, :vtd_abbrv)"
    "ON CONFLICT (county_id, voter_reg_num) DO NOTHING;";
const char *sqlite_commit_transaction = "COMMIT TRANSACTION;";


int read_line_into_nc_voter_data(char *line, ssize_t line_len, void *data_p, [[maybe_unused]] sqlite3_stmt *pStmt_arr[])
{
    struct nc_voter_data *voter_data_p = data_p;
    char *varchar_arr[66] = { NC_VOTER_VARCHARS_FIELD_LIST((*voter_data_p)) };
    char *temp;
    size_t len;

    int n = sscanf(line, "\"%d\"\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t]\t\"%[^\t\r\n]", NC_VOTER_FIELD_LIST((*voter_data_p)));
    if (!n)
    {
        perror("Issue in scanning record line for county table");
        return -1;
    }
    for (int i = 0; i < 66; i++)
    {
        temp = varchar_arr[i];
        if ((len = strlen(temp)))
            temp[len - 1] = '\0';
    }
    return 0;
}

int for_each_line(FILE *fp, void *context_p, size_t callback_count, int (*callbacks[])(char *, ssize_t, [[maybe_unused]] void *, [[maybe_unused]] sqlite3_stmt *[]), sqlite3_stmt *pStmt_arr[])
{
    ssize_t linelen;
    char *line = NULL;
    size_t linecap = 0;
    if (fp == NULL)
    {
        fprintf(stderr, "Error parsing input path\n");
        return -1;
    }
    linelen = getline(&line, &linecap, fp);
    if (linelen < 0)
    {
        fprintf(stderr, "EOF encountered on read of header line\n");
        return -1;
    }
    while ((linelen = getline(&line, &linecap, fp) > 0))
    {
        for (size_t i = 0; i < callback_count; i++)
        {
            if (callbacks[i](line, linelen, context_p, pStmt_arr))
            {
                return -1;
            }
        }
    }
    return 0;
}

#define HANDLE_SQLITE_ERR(RC, ERR, FLAG) if ((RC) != SQLITE_OK) { fprintf(stderr, "SQL error: %s\n", ERR); sqlite3_free(ERR); ERR = NULL; FLAG = -1;}

int update_db(char *line, ssize_t linelen, [[maybe_unused]] void *context_p, [[maybe_unused]] sqlite3_stmt *pStmt_arr[])
{
    static bool counties[1024] = {false};
    struct nc_voter_data *voter_data_p = context_p;
    int index, rc, result = 0;
    char *nc_voter_sql_params[66] = { NC_VOTER_SQL_VARCHAR_PARAMS_LIST };
    char *nc_voter_varchar_vals[66] = { NC_VOTER_VARCHARS_FIELD_LIST((*voter_data_p)) };
    for (int j = 0; j < 7; j++)
    {
        rc = sqlite3_reset(pStmt_arr[j]);
        index = sqlite3_bind_parameter_index(pStmt_arr[j], ":county_id");
        if (index)
        {
            rc = sqlite3_bind_int(pStmt_arr[j], index, voter_data_p->county_id);
            if (2 == j && voter_data_p->county_id > 0 && voter_data_p->county_id < 1024)
            {
                if (counties[voter_data_p->county_id])
                    continue;
                else
                    counties[voter_data_p->county_id] = true;
            }
            if (SQLITE_OK != rc)
            {
                fprintf(stderr, "Issue in sqlite3_bind_int, code %d, error message %s\n", rc, sqlite3_errmsg(main_db));
                return -1;
            }
        }
        for (int i = 0; i < 66; i++)
        {
            index = sqlite3_bind_parameter_index(pStmt_arr[j], nc_voter_sql_params[i]);
            if (!index)
                continue;
            if ('\0' != nc_voter_varchar_vals[i][0])
                rc = sqlite3_bind_text(pStmt_arr[j], index, nc_voter_varchar_vals[i], -1, SQLITE_STATIC);
            else
                rc = sqlite3_bind_null(pStmt_arr[j], index);
        }
        rc = sqlite3_step(pStmt_arr[j]);
        if (SQLITE_DONE != rc)
        {
            fprintf(stderr, "Issue in sqlite3_step, code %d, error message %s\n", rc, sqlite3_errmsg(main_db));
            return -1;
        }
    }
    return 0;
}


int enforce_schema(sqlite3 *db)
{
    char *err_msg = NULL;
    int rc, saw_err = 0;

    rc = sqlite3_exec(db, COUNTY_SCHEMA, NULL, NULL, &err_msg);
    HANDLE_SQLITE_ERR(rc, err_msg, saw_err)
    rc = sqlite3_exec(db, VOTER_SCHEMA, NULL, NULL, &err_msg);
    HANDLE_SQLITE_ERR(rc, err_msg, saw_err)
    rc = sqlite3_exec(db, STATUS_SCHEMA, NULL, NULL, &err_msg);
    HANDLE_SQLITE_ERR(rc, err_msg, saw_err)
    rc = sqlite3_exec(db, REASON_SCHEMA, NULL, NULL, &err_msg);
    HANDLE_SQLITE_ERR(rc, err_msg, saw_err)
    rc = sqlite3_exec(db, REGISTRATION_SCHEMA, NULL, NULL, &err_msg);
    HANDLE_SQLITE_ERR(rc, err_msg, saw_err)
    return saw_err;
}

int main(int argc, char *argv[])
{
    int (*callbacks[2])(char *, ssize_t, [[maybe_unused]] void *, [[maybe_unused]] sqlite3_stmt *[]) = { 
        read_line_into_nc_voter_data, update_db 
    };
    struct nc_voter_data voter_data;
    sqlite3 *db;
    int rc;
    const char *sqlite_update_sequence[7] = { sqlite_begin_transaction, sqlite_update_county, 
        sqlite_update_voter, sqlite_update_status, sqlite_update_reason, sqlite_update_registration,
        sqlite_commit_transaction };
    sqlite3_stmt *pStmt_arr[7];

    if (argc != 4)
    {
        errx(EXIT_FAILURE, "Pass file type, path, and db path to gendb\n");
    }
    rc = sqlite3_open_v2(argv[3], &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return EXIT_FAILURE;
    }
    main_db = db;
    enforce_schema(db);
    switch (argv[1][0])
    {
        case 'r':
            break;
        default:
            errx(EXIT_FAILURE, "Unsupported file type\n");
    }
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL)
        errx(EXIT_FAILURE, "Error parsing input path\n");
    for (int i = 0; i < 7; i++)
    {
        rc = sqlite3_prepare_v3(db, sqlite_update_sequence[i], -1, SQLITE_PREPARE_PERSISTENT, &pStmt_arr[i], NULL);
        if (rc)
        {
            fprintf(stderr, "Issue in sqlite3_prepare_v3: %s\n", sqlite3_errmsg(db));
            fclose(fp);
            for (int j = 0; j <= i; j++)
                sqlite3_finalize(pStmt_arr[j]);
            sqlite3_close_v2(db);
            return EXIT_FAILURE;
        }
    }
    if (0 > for_each_line(fp, &voter_data, 2U, callbacks, pStmt_arr))
    {
        errx(EXIT_FAILURE, "Error doing callbacks: %s\n", sqlite3_errmsg(db));
    }
    fclose(fp);
    for (int j = 0; j < 7; j++)
        sqlite3_finalize(pStmt_arr[j]);
    sqlite3_close_v2(db);
    return EXIT_SUCCESS;
}
