program snctest
float v;
assign v to "leige:xxxExample";
monitor v;

ss ss1
{
	state low
	{
	    when(v>5.0)
	    {
		printf("changing to high\n");
	    } state high
	    when(delay(.1)) 
	    {
	    } state low
	}
	state high
	{
	    when(v<=5.0)
	    {
		printf("changing to low\n");
	    } state low
	    when(delay(.1))
	    {
	    } state high
	}
}
