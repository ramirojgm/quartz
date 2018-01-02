var db = getProvider();

httpGet("/client/get.json",function(data) {
	try
	{
		return {
			done = true,
			data: db.query("SELECT * FROM client")
		};
	}
	catch(e)
	{
		log("Exception:",e,"\n");
		return {
			done = false,
			message = e
		};
	}
});

httpPost("/client/insert",function(data) {
	try {
		db.exec("INSERT INTO client (name,address) VALUES (@name,@address)",data);
		return {
			done = true,
			data = db.getLastId()
		};
	}
	catch(e)
	{
		log("Exception:",e,"\n");
		return {
			done = false,
			message = e
		};
	}
});

httpPost("/client/update",function(data) {
	try {
		db.exec("UPDATE client SET name = @name, address = @address WHERE idclient = @idclient",data);
		return {
			done = true,
			data = null
		};
	}
	catch(e)
	{
		log("Exception:",e,"\n");
		return {
			done = false,
			message = e
		};
	}
});

httpPost("/client/delete",function(data) {
	try {
		db.exec("DELETE FROM client WHERE idclient = @idclient",data);
		return {
			done = true,
			data = null
		};
	}
	catch(e)
	{
		log("Exception:",e,"\n");
		return {
			done = false,
			message = e
		};
	}
});