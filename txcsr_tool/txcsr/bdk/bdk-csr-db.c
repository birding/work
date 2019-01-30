#include <bdk.h>
#include <stdio.h>
#define info_print(args ...) printf(args)

extern const __bdk_csr_db_map_t __bdk_csr_db[];
extern const __bdk_csr_db_type_t __bdk_csr_db_csr[];
extern const uint16_t __bdk_csr_db_fieldList[];
extern const __bdk_csr_db_field_t __bdk_csr_db_field[];
extern const int __bdk_csr_db_range[];
extern const char __bdk_csr_db_string[];
extern const uint64_t __bdk_csr_db_number[];

extern int __bdk_csr_db_string_count( void );

/* This will point to one of the "data" arrays listed in __bdk_csr_db. The
    numbers index __bdk_csr_db_csr[] to find the CSR. The last one is zero,
    which has a null CSR at __bdk_csr_db_csr[0] */
static const int16_t *csr_list = NULL;

const int16_t * get_db_list(char * modelstr)
{
        const __bdk_csr_db_map_t *map = __bdk_csr_db;
		
#ifdef _X86_HOST_
	int iCpu;
	uint32_t model=0;

	if(modelstr != NULL){
		if (strncasecmp(modelstr, "cn83xx", strlen(modelstr)) == 0){
			model = CAVIUM_CN83XX_PASS1_X;
		}
		//printf("[%d]%s\n", __LINE__, modelstr);
	}

	if(model == 0){		
		printf("Select CPU model:\n\t[1]CN81XXP1_0\n\t[2]CN83XXP1_0\n"
				">> ");
		scanf("%d",&iCpu);
		switch(iCpu)
		{
			case 2:
				model=CAVIUM_CN83XX_PASS1_X;
				break;				
			case 1:
				model=CAVIUM_CN81XX_PASS1_X;
				break;
			default:
				return NULL;
		}
	}
        while (map->model && (map->model != model))
            map++;
#else
        while (map->model && !CAVIUM_IS_MODEL(map->model))
            map++;
#endif
        if (map->model == 0)
        {
            bdk_error("CSR lookup can't find this chip\n");
            return NULL;
        }
        csr_list = map->data;	
	return csr_list;
}

void display_range(const __bdk_csr_db_type_t  *db)
{
	int i=0;

	for(i=0; i<BDK_CSR_DB_MAX_PARAM; i++){
		if(db->range[i])
		{
			if(i == 0){
				printf(" a[");
			}else if(i==1){
				printf(" b[");
			}else if(i==2){
				printf(" g[");
			}else if(i==3){
				printf(" c[");			
			}else{
				printf(" error");
			}

		    int range_len = __bdk_csr_db_range[db->range[i]];
			if(range_len==2){
				printf("%d-%d]", __bdk_csr_db_range[db->range[i]+1], 
							__bdk_csr_db_range[db->range[i]+2]);
			}
		}
	}
}

void display_range_list(const __bdk_csr_db_type_t  *db, char * pre, int index)
{
	char spacer[128];

	if(index == BDK_CSR_DB_MAX_PARAM){
		printf("%s\n", pre);
		return;
	}

	memset(spacer, 0, sizeof(spacer));
	switch(index){
		case 0: sprintf(spacer, "-a"); break;
		case 1: sprintf(spacer, "-b"); break;
		case 2: sprintf(spacer, "-g"); break;
		case 3: sprintf(spacer, "-c"); break;
	}
	
	if(db->range[index])
	{
		int range_len = __bdk_csr_db_range[db->range[index]];
		if(range_len==2){
			int i;
			for(i= __bdk_csr_db_range[db->range[index]+1];
				i<= __bdk_csr_db_range[db->range[index]+2]; i++){
				char pbuf[128];

				memset(pbuf, 0, sizeof(pbuf));
				sprintf(pbuf, "%s %s %d ", pre, spacer, i);
				display_range_list(db, pbuf, index+1);
			}
		}
	}else{
		printf("%s\n", pre);
	}
	
}

/*
 * mode=0: list info 
 */
void display_matched_result(char * mode, char * app, int index)
{
	const __bdk_csr_db_type_t *db = &__bdk_csr_db_csr[csr_list[index]];
	const char * ptr = __bdk_csr_db_string + db->name_index*2;

	if (strncasecmp(mode, "mode0", strlen(mode)) == 0){
		printf("\n%s ",  ptr);			
		for (int i=0; i<BDK_CSR_DB_MAX_PARAM; i++)
		{
			if(db->range[i]){
			    int range_len = __bdk_csr_db_range[db->range[i]];
			    int j = db->range[i]+1;
			    while (range_len)
			    {
			        range_len -= 2;
			        printf("[%d %d] ", __bdk_csr_db_range[j], __bdk_csr_db_range[j+1]);
			        j=j+2;
			    }					
			}

		}
	}else if (strncasecmp(mode, "mode1", strlen(mode)) == 0){
	// ./txcsr_tool --listregd1 usb
	// ./txcsr_tool USBDRDX_MSIX_PBAX a[0-1] b[0-0]
	// ./txcsr_tool USBDRDX_UAHC_CONFIG a[0-1]
	// --finish
		char pbuf[128];

		memset(pbuf, 0, sizeof(pbuf));
		sprintf(pbuf, "%s %s", app, ptr);
		printf("\n%s",pbuf);
		
		display_range(db);
		
	}else if (strncasecmp(mode, "mode2", strlen(mode)) == 0){
		//./txcsr_tool --listregd2 sata
		//--finish
		char pbuf[128];

		memset(pbuf, 0, sizeof(pbuf));
		sprintf(pbuf, "%s %s -d", app, ptr);
		
		display_range_list(db, pbuf, 0);
	}else{
		printf("list mode undefined!\n");
	}
	
}

#define version_str "ver:1.1.2 "

void name_suggestion_for_details(char *app,  char * model, char * mode, char *name)
{
	int idx=0, count=0;

	printf("%s\n\n", version_str);

	csr_list = get_db_list(model);
	if(NULL == csr_list)
		return;

    if (NULL == name){
        return ;
    	}
		
	while(1)
	{
		if(csr_list[idx]==0)
			break;
		
	    const __bdk_csr_db_type_t *db = &__bdk_csr_db_csr[csr_list[idx]];		
	    if (db->width)
	    {
	    	const char * ptr = __bdk_csr_db_string + db->name_index*2;
	        if (strncasecmp(name, ptr, strlen(name)) == 0)
        	{        		
			display_matched_result(mode, app, idx);
			count++;
        	}
	    }
	
	    idx++;
	}
	
	printf("\nTotal number of register count: %d, matched %d\n",idx, count);
	return ;
}


/*
 *@name : Block name for which suggestions will be provided.
 * eg. SATAX
 * */
void name_suggestion(char *name)
{
	char const *ptr;

	int count;
	ptr = __bdk_csr_db_string;
	char separators[] = "\" _";
	char prefix[20];
	char sample[30];
	char sample1[30];
	count = __bdk_csr_db_string_count();
	info_print("Total number of register count: %d\n",count);
	while ( count > 1 ) {
		strncpy(sample, ptr, sizeof(sample));
		strncpy(sample1, ptr, sizeof(sample1));
		char *p1 = strtok(sample, separators);

		strncpy(prefix, p1, sizeof(prefix));

		if (strcmp(prefix, name) == 0)
			info_print("%s\n", sample1);

		while (*ptr != '\0') {
			ptr++;
			count--;
		}
		while (*ptr == '\0') {
			ptr++;
			count--;
		}
	}
	printf("\n");
}


/**
 * Check if a supplied value is in a CSR range
 *
 * @param rangeid Range ID to check against
 * @param value   Value to check range on
 * @param next    If supplied, this is filled with the next valid value in the range. If the
 *                current value is the last one, then this is set to -1. If value
 *                is invalid, then this will be set to -1.
 *
 * @return Zero if the supplied value is valid. Negative if it is not
 */
static int __bdk_csr_check_range(int rangeid, int value, int *next)
{
    /* Ranges are stored as a length followed by min/max pairs. Length is always
        a multiple of 2 */
    int range_len = __bdk_csr_db_range[rangeid];
    int i = rangeid+1;
    while (range_len)
    {
        range_len -= 2;
        if ((value >= __bdk_csr_db_range[i]) && (value <= __bdk_csr_db_range[i+1]))
        {
            if (next)
            {
                if (value + 1 <= __bdk_csr_db_range[i+1])
                    *next = value + 1;
                else if (range_len)
                    *next = __bdk_csr_db_range[i+2];
                else
                    *next = -1;
            }
            return 0;
        }
        i += 2;
    }
    if (next)
        *next = -1;
    return -1;

}


/**
 * Given a name, find the CSR associated with it.
 *
 * @param name   Name to find. It can contain extra range info in parenthesis
 * @param offset Filled with the argument in parenthesis from the name
 * @param block  Filled with the argument in parenthesis from the name
 *
 * @return Index into csr_list table, or -1 on failure
 */
int __bdk_csr_lookup_index(const char *name, int inf_id[])
{
    if (!csr_list)
    {
        /* Search the CSR DB for a csr map for this chip */
        const __bdk_csr_db_map_t *map = __bdk_csr_db;
        while (map->model && !CAVIUM_IS_MODEL(map->model))
            map++;
        if (map->model == 0)
        {
            bdk_error("CSR lookup can't find this chip\n");
            return -1;
        }
        csr_list = map->data;

    }

    /* Return the first DB entry if no name was supplied */
    if (name == NULL)
        return 0;

    /* Extract the CSR index info from the name */
    char *compare = strdup(name);

    /* Search the DB for this CSR */
    int index=0;
    const __bdk_csr_db_type_t *db = &__bdk_csr_db_csr[csr_list[index]];
    while (db->width)
    {
        if (strcasecmp(compare, __bdk_csr_db_string + db->name_index*2) == 0)
            break;
        index++;
        db = &__bdk_csr_db_csr[csr_list[index]];
    }
    free(compare);
    /* Fail if we reached the end of the list without finding it */
    if (db->width == 0)
        return -1;

    /* Fail if range in invalid */
    for (int i=0; i<BDK_CSR_DB_MAX_PARAM; i++)
    {
        if (__bdk_csr_check_range(db->range[i], inf_id[i], NULL))
        {
            bdk_error("CSR lookup with invalid offset\n");
            return -1;
        }
    }

    return index;
}


/**
 * Given a name, find the CSR associated with it.
 *
 * @param name   Name to find. It can contain extra range info in parenthesis
 * @param offset Filled with the argument in parenthesis from the name
 * @param block  Filled with the argument in parenthesis from the name
 *
 * @return Non NULL on success
 */
const __bdk_csr_db_type_t *__bdk_csr_lookup(const char *name, int inf_id[])
{
    int db_index = __bdk_csr_lookup_index(name, inf_id);
    /* Fail if we can't find the CSR */
    if (db_index == -1)
    {
        bdk_error("CSR lookup can't find this CSR\n");
        return NULL;
    }
    return &__bdk_csr_db_csr[csr_list[db_index]];
}


/**
 * Figure out the address of a CSR given its DB entry and range arguments
 *
 * @param db     DB entry for CSR
 * @param offset Offset inside block
 * @param block  Block index
 *
 * @return Address of CSR
 */
static uint64_t __bdk_csr_lookup_address(const __bdk_csr_db_type_t *db, int params[])
{
    uint64_t address = __bdk_csr_db_number[db->base_index];
    for (int i=0; i<BDK_CSR_DB_MAX_PARAM; i++)
    {
        if (params[i] != -1)
            address += __bdk_csr_db_number[db->param_inc[i]] * params[i];
    }
    return address;
}


/**
 * Decode a CSR in a human readable format
 *
 * @param name   Name of CSR to decode
 * @param value  Value to decode
 *
 * @return Zero on success, negative on failure
 */
int bdk_csr_decode(const char *name, uint64_t value, int inf_id[])
{
    int params[BDK_CSR_DB_MAX_PARAM] = {-1, -1, -1, -1};
    const __bdk_csr_db_type_t *db = __bdk_csr_lookup(name, inf_id);
    int num_fields;

    /* Fail if we can't find the CSR */
    if (!db)
        return -1;

    /* Print the official CSR name */
    printf("\nRegister : %s\n", __bdk_csr_db_string + db->name_index*2);
    if (params[0] != -1)
    {
        printf("(%d", params[0]);
        for (int i=1; i<BDK_CSR_DB_MAX_PARAM; i++)
        {
            if (params[i] != -1)
                printf(",%d", params[i]);
            else
                break;
        }
        printf(")");
    }

    /* Print the address and value */

    num_fields = __bdk_csr_db_fieldList[db->field_index];
    while (num_fields--)
    {
        int field = __bdk_csr_db_fieldList[db->field_index + 1 + num_fields];
        const char *field_name = __bdk_csr_db_string + __bdk_csr_db_field[field].name_index*2;
        int start_bit = __bdk_csr_db_field[field].start_bit;
        int stop_bit = __bdk_csr_db_field[field].stop_bit;
        int size_bits = stop_bit - start_bit + 1;
        uint64_t v = (value >> start_bit);
        if(size_bits < 64)
            v = v & ~((~0x0ull) << size_bits);
        if (size_bits == 1)
            printf("  [   %2d] %-20s = %10llu (0x%llx)\n", start_bit, field_name,
                (unsigned long long)v, (unsigned long long)v);
        else
            printf("  [%2d:%2d] %-20s = %10llu (0x%llx)\n", stop_bit, start_bit, field_name,
                (unsigned long long)v, (unsigned long long)v);
    }
    return 0;
}


/**
 * Lookup a field by knowning its CSR and start_bit. By starting
 * at zero, you can walk all fields.
 *
 * @param csr_name   CSR to lookup field for
 * @param field_start_bit
 *                   Starting bit of field
 * @param field_name Constant character pointer to receive the address of the field name.
 *
 * @return Width of the field, or negative on failure.
 */
int bdk_csr_field(const char *csr_name, int field_start_bit, const char **field_name)
{
    int params[BDK_CSR_DB_MAX_PARAM] = {-1, -1, -1, -1};
    const __bdk_csr_db_type_t *db = __bdk_csr_lookup(csr_name, params);
    int num_fields;
    int i;

    /* Fail if we can't find the CSR */
    if (!db)
        return -1;

    num_fields = __bdk_csr_db_fieldList[db->field_index];
    i = db->field_index + 1;
    while (num_fields--)
    {
        int field = __bdk_csr_db_fieldList[i++];
        int start_bit = __bdk_csr_db_field[field].start_bit;
        if (start_bit == field_start_bit)
        {
            int stop_bit = __bdk_csr_db_field[field].stop_bit;
            *field_name = __bdk_csr_db_string + __bdk_csr_db_field[field].name_index*2;
            return stop_bit - start_bit + 1;
        }
    }
    return -1;
}

/**
 * Get a CSR name. This is menat to be used to iterate through all
 * possible CSR names.
 *
 * @param last_name Get the CSR following this name. Supply NULL for the first CSR
 * @param buffer    Buffer to fill with the CSR name
 *
 * @return Zero on success, negative on list end or failure
 */
int bdk_csr_get_name(const char *last_name, char *buffer)
{
    int params[BDK_CSR_DB_MAX_PARAM] = {-1, -1, -1, -1};
    const __bdk_csr_db_type_t *db;

    /* Find our current DB spot */
    int db_index = __bdk_csr_lookup_index(last_name, params);
    if (db_index == -1)
        return -1;
    db = &__bdk_csr_db_csr[csr_list[db_index]];

    /* We we are continuing we need to process the range data and figure out what is next */
    if (last_name)
    {
        int need_next_csr = 1;
        for (int p=BDK_CSR_DB_MAX_PARAM-1; p>=0; p--)
        {
            /* Skip unused params */
            if (params[p] == -1)
                continue;
            int next = -1;
            /* Make sure the current param is valid */
            if (__bdk_csr_check_range(db->range[p], params[p], &next))
                return -1;
            /* Special check alert. CN88XX TNS block has two registers that
               read/write the packet memory. These have huge address ranges, so
               only return index 0 for these */
            {
                const char *name = __bdk_csr_db_string + db->name_index * 2;
                if (strncmp(name, "TNS_PM_", 7) == 0)
                    next = -1;
            }
            /* If there isn't a next, we may need to increment the next param */
            if (next == -1)
            {
                /* This param goes back to the lowest index */
                params[p] = __bdk_csr_db_range[db->range[p]+1];
                if (p>0)
                    continue;
                else
                {
                    need_next_csr = 1;
                    break;
                }
            }
            else
            {
                params[p] = next;
                need_next_csr = 0;
                break;
            }
        }
        if (need_next_csr)
        {
            /* End of params, so skip to next CSR */
            db = &__bdk_csr_db_csr[csr_list[++db_index]];
            for (int i=0; i<BDK_CSR_DB_MAX_PARAM; i++)
                params[i] = __bdk_csr_db_range[db->range[i]+1];
        }
    }
    else
    {
        /* Fill in params for the first DB entry */
        for (int i=0; i<BDK_CSR_DB_MAX_PARAM; i++)
            params[i] = __bdk_csr_db_range[db->range[i]+1];
    }

    /* End of list, return failure */
    if (db->width == 0)
        return -1;

    /* Fill in the next name */
    if (buffer)
    {
        buffer += sprintf(buffer, "%s", __bdk_csr_db_string + db->name_index*2);
        if (params[0] != -1)
        {
            buffer += sprintf(buffer, "(%d", params[0]);
            for (int i=1; i<BDK_CSR_DB_MAX_PARAM; i++)
            {
                if (params[i] != -1)
                    buffer += sprintf(buffer, ",%d", params[i]);
                else
                    break;
            }
            buffer += sprintf(buffer, ")");
        }
    }

    return 0;
}

/**
 * Do a CSR read based on the string name of a CSR
 *
 * @param node   Node to use in a Numa setup. Can be an exact ID or a special
 *               value.
 * @param name   Name of CSR to read
 *
 * @return Zero on failure or the CSR value
 */
uint64_t bdk_csr_read_by_name(const char *name, int inf_id[])
{
	//int params[BDK_CSR_DB_MAX_PARAM] = {-1, -1, -1, -1};
	const __bdk_csr_db_type_t *db = __bdk_csr_lookup(name, inf_id);

	if (!db) {
		printf("\n DataBase not found.\n");
		exit(0);
	}
	return __bdk_csr_lookup_address(db, inf_id);
}

/**
 * Read a core system register from a different node or core
 *
 * @param node   Node to read from
 * @param core   Core to read
 * @param regnum Register to read in MRS encoding
 *
 * @return Register value
 */
uint64_t bdk_sysreg_read(const char *name, int inf_id[], int core)
{
    uint64_t regnum;
    
    regnum = bdk_csr_read_by_name(name,inf_id);
    
    /* Addresses indicate selects as follows:
        select 3,4,14,2,3
     == 0x03040e020300
           | | | | |^--- 1 if is E2H duplicated register
           | | | |^^-- fifth select
           | | |^^-- fourth select
           | |^^-- third select
           |^^-- second select
          ^^-- first select */
    uint64_t first = (regnum >> 40) & 0xff;
    uint64_t second = (regnum >> 32) & 0xff;
    uint64_t third = (regnum >> 24) & 0xff;
    uint64_t fourth = (regnum >> 16) & 0xff;
    uint64_t fifth = (regnum >> 8) & 0xff; 
    uint64_t regid = ((first & 3) << 14) | (second << 11) | (third << 7) | (fourth << 3) | fifth;

    /* Note this requires DAP_IMP_DAR[caben] = 1 */
    uint64_t address = 1ull<<47;
    address |= 0x7Bull << 36;
    address |= core << 19;
    address |= regid << 3;
    return address;
}

