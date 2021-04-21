#include <mariadb/mysql.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printusage(){
        printf(
"#Please configure using environment like:\n"
"export DB_SERVER=localhost\n"
"export DB_SERVER_USERNAME=phoenix\n"
"export DB_SERVER_PASSWORD=phoenix\n"
"export DB_DATABASE=phoenix\n"
"export NEW_CUSTOMERS_ID_RESERVE=4\n"
"to Configure PureSQL user rights\n");
return 0;
}
/* pointer to server connection details */
struct connection_details
{
    char *server;
    char *user;
    char *password;
    char *database;
};

/* function for connecting and returning pointer */
MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
    /* creating new instance */
    MYSQL *conn = mysql_init(NULL);

    /* connect to the database with the details */
    if (!mysql_real_connect(conn, mysql_details.server, mysql_details.user,
        mysql_details.password, mysql_details.database, 0, NULL, 0))
    {
        printf("Conection error : %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
};

/* function for performing queries */
MYSQL_RES* mysql_perform_query(MYSQL *conn, char *query)
{
    if (mysql_query(conn, query))
    {
        printf("MySQL query error : %s\n", mysql_error(conn));
        exit(1);
    }
    return mysql_use_result(conn);
};

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int new_customers_id_reserve = 0;
    int last_empty_customers_id = 0;

    // assign details to database
    struct connection_details mysql_conn_string;
    mysql_conn_string.server = getenv("DB_SERVER");
    mysql_conn_string.user = getenv("DB_SERVER_USERNAME");
    mysql_conn_string.password = getenv("DB_SERVER_PASSWORD");
    mysql_conn_string.database = getenv("DB_DATABASE");
    new_customers_id_reserve = getenv("NEW_CUSTOMERS_ID_RESERVE") ? atoi(getenv("NEW_CUSTOMERS_ID_RESERVE")) : 4;
    printf("new_customers_id_reserve: %d\n", new_customers_id_reserve);
    if(strlen(mysql_conn_string.database) && 
    strlen(mysql_conn_string.server) &&
    strlen(mysql_conn_string.user) &&
    strlen(mysql_conn_string.password)
    ){

        conn = mysql_connection_setup(mysql_conn_string);

        if(conn) {
            int last_real_customer = 0;
            res = mysql_perform_query(conn, "SELECT customers_id FROM customers_real ORDER BY customers_id DESC LIMIT 1;");
            if ((row = mysql_fetch_row(res)) !=NULL){
                last_real_customer = atoi(row[0]);
            }
            mysql_free_result(res);
            res = mysql_perform_query(conn, "SELECT customers_id FROM last_empty_customers_id;");
            if ((row = mysql_fetch_row(res)) !=NULL){
                last_empty_customers_id = atoi(row[0]);
            }
            mysql_free_result(res);

            if (last_real_customer+new_customers_id_reserve > last_empty_customers_id){
              for (int user_id=last_empty_customers_id+1;user_id < last_empty_customers_id+new_customers_id_reserve;++user_id){
                char query[10000] = "";
                strcpy(sprintf("CREATE USER %s_%d@%s", mysql_conn_string.database, user_id, mysql_conn_string.server),query);
                printf("query: %s", query);
                mysql_perform_query(conn, query);
/*
    mysql "GRANT USAGE on ${D}.* to ${D}_${N}@${H}"
#debug only!!!
#    mysql "GRANT ALL on ${D}.* to ${D}_${N}@${H}"

    #TODO:otestovat contact_us!!!
    mysql "grant INSERT on $D.action_recorder to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.address_book to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.address_format to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.advert to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.advert_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.article_reviews to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.article_reviews_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_blog to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_description to ${D}_${N}@${H}"
    mysql "grant UPDATE (articles_viewed) on $D.articles_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_to_topics to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_xsell to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.authors to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.authors_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.cache to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.categories to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.categories_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.configuration to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.configuration_group to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.countries to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.currencies to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.customer_data_groups to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.customer_data_groups_sequence to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE on $D.customers to ${D}_${N}@${H}"
    mysql -h${H} -u${U} -p${P} ${D} -e "grant SELECT on ${D}.customers_anonym to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.customers_basket to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.customers_basket_attributes to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE on $D.customers_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.geo_zones to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.hooks to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.information to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.information_group to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.languages to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.last_empty_customers_id to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.manufacturers to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.manufacturers_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.newsletters to ${D}_${N}@${H}"
#delete ne? TODO
    mysql "grant SELECT, INSERT, UPDATE on $D.orders to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.orders_products to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.orders_products_attributes to ${D}_${N}@${H}"
#TODO jen select?
    mysql "grant SELECT on $D.orders_products_download to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.orders_status to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT on $D.orders_status_history to ${D}_${N}@${H}"
#TODO??
    mysql "grant SELECT, INSERT, DELETE on $D.orders_total to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.pages to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.pages_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products to ${D}_${N}@${H}"
    mysql "grant UPDATE (products_quantity) on $D.products to ${D}_${N}@${H}"
    mysql "grant UPDATE (products_ordered) on $D.products to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_attributes to ${D}_${N}@${H}"
    mysql "grant UPDATE (options_values_quantity) on $D.products_attributes to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_attributes_download to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_description to ${D}_${N}@${H}"
##BACHA! otestovat
    mysql "grant UPDATE (products_viewed) on $D.products_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_images to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.products_notifications to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options_values to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options_values_to_products_options to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_to_categories to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.reviews to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.reviews_description to ${D}_${N}@${H}"
##?????!!!! je treba
    mysql "grant SELECT on $D.sec_directory_whitelist to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.sessions to ${D}_${N}@${H}"

    mysql "grant SELECT on $D.specials to ${D}_${N}@${H}"
    mysql "grant UPDATE (status, date_status_change) on $D.specials to ${D}_${N}@${H}"
##?treba
    mysql "grant SELECT on $D.tax_class to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.tax_rates to ${D}_${N}@${H}"
#???? update?
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.testimonials to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.testimonials_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.topics to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.topics_description to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.whos_online to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.zones to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.zones_to_geo_zones to ${D}_${N}@${H}"
*/
              }
            } 




            printf("last_real_customer: %d \n", last_real_customer);

    // reset objects
    mysql_free_result(res);
    mysql_close(conn);


        } else {
            printf("Curent used values:\nDB_SERVER: %s DB_SERVER_USERNAME: %s, DB_SERVER_PASSWORD: %s, DB_DATABASE: %s\n",mysql_conn_string.server, mysql_conn_string.user, mysql_conn_string.password, mysql_conn_string.database);
            printusage();
            exit(1);
        }
        // connecting to database with details
  
            // display query
//            printf("MySQL Tables in mysql database:\n");
//            while ((row = mysql_fetch_row(res)) !=NULL)
//            {
//                printf("%s\n", row[0]);
//            }


    } else {
        printusage();
        exit(1);
    }




    return 0;
}



/*
//NEW_CUSTOMERS_ID_RESERVE=4
//LAST_REAL_CUSTOMER=`mysql --column-names=FALSE -h${H} -u${RU} -p${RP} $D -e "SELECT customers_id FROM customers_real ORDER BY customers_id DESC LIMIT 1"`
//echo '$LAST_REAL_CUSTOMER:' $LAST_REAL_CUSTOMER

//LAST_EMPTY_CUSTOMERS_ID=`mysql --column-names=FALSE -h${H} -u${RU} -p${RP} $D -e "SELECT customers_id FROM last_empty_customers_id"`
//echo 'LAST_EMPTY_CUSTOMERS_ID:' $LAST_EMPTY_CUSTOMERS_ID
if [[ $((LAST_REAL_CUSTOMER+NEW_CUSTOMERS_ID_RESERVE)) -le LAST_EMPTY_CUSTOMERS_ID ]]
then
  echo exit
	exit
else
  N=$((LAST_EMPTY_CUSTOMERS_ID+1))
  while [ $N -le $((LAST_EMPTY_CUSTOMERS_ID+NEW_CUSTOMERS_ID_RESERVE)) ]
  do
    mysql "CREATE USER ${D}_${N}@${H}"
    mysql "GRANT USAGE on ${D}.* to ${D}_${N}@${H}"
#debug only!!!
#    mysql "GRANT ALL on ${D}.* to ${D}_${N}@${H}"

    #TODO:otestovat contact_us!!!
    mysql "grant INSERT on $D.action_recorder to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.address_book to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.address_format to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.advert to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.advert_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.article_reviews to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.article_reviews_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_blog to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_description to ${D}_${N}@${H}"
    mysql "grant UPDATE (articles_viewed) on $D.articles_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_to_topics to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.articles_xsell to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.authors to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.authors_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.cache to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.categories to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.categories_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.configuration to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.configuration_group to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.countries to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.currencies to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.customer_data_groups to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.customer_data_groups_sequence to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE on $D.customers to ${D}_${N}@${H}"
    mysql -h${H} -u${U} -p${P} ${D} -e "grant SELECT on ${D}.customers_anonym to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.customers_basket to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.customers_basket_attributes to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE on $D.customers_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.geo_zones to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.hooks to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.information to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.information_group to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.languages to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.last_empty_customers_id to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.manufacturers to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.manufacturers_info to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.newsletters to ${D}_${N}@${H}"
#delete ne? TODO
    mysql "grant SELECT, INSERT, UPDATE on $D.orders to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.orders_products to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.orders_products_attributes to ${D}_${N}@${H}"
#TODO jen select?
    mysql "grant SELECT on $D.orders_products_download to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.orders_status to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT on $D.orders_status_history to ${D}_${N}@${H}"
#TODO??
    mysql "grant SELECT, INSERT, DELETE on $D.orders_total to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.pages to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.pages_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products to ${D}_${N}@${H}"
    mysql "grant UPDATE (products_quantity) on $D.products to ${D}_${N}@${H}"
    mysql "grant UPDATE (products_ordered) on $D.products to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_attributes to ${D}_${N}@${H}"
    mysql "grant UPDATE (options_values_quantity) on $D.products_attributes to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_attributes_download to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_description to ${D}_${N}@${H}"
##BACHA! otestovat
    mysql "grant UPDATE (products_viewed) on $D.products_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_images to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.products_notifications to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options_values to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_options_values_to_products_options to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.products_to_categories to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.reviews to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.reviews_description to ${D}_${N}@${H}"
##?????!!!! je treba
    mysql "grant SELECT on $D.sec_directory_whitelist to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.sessions to ${D}_${N}@${H}"

    mysql "grant SELECT on $D.specials to ${D}_${N}@${H}"
    mysql "grant UPDATE (status, date_status_change) on $D.specials to ${D}_${N}@${H}"
##?treba
    mysql "grant SELECT on $D.tax_class to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.tax_rates to ${D}_${N}@${H}"
#???? update?
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.testimonials to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.testimonials_description to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.topics to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.topics_description to ${D}_${N}@${H}"
    mysql "grant SELECT, INSERT, UPDATE, DELETE on $D.whos_online to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.zones to ${D}_${N}@${H}"
    mysql "grant SELECT on $D.zones_to_geo_zones to ${D}_${N}@${H}"

echo NNN: ${N}
	  let N+=1
  done
  mysql "FLUSH PRIVILEGES"
  mysql "UPDATE last_empty_customers_id SET customers_id=$((N-1))"
fi

exit

#master admin
grant all privileges on mydatabase.* to masteradmin@dbserver identified by 'masteradminpassword' with grant option;
#catalog side user
grant usage on mydatabase.* to cataloguser@dbserver identified by 'cataloguserpassword';


*/