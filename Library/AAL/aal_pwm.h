/**
 * @file
 * @brief	AAL_PWM Header File
 * */
#ifndef __AAL_PWM_H
#define __AAL_PWM_H

#ifdef	DOXYGEN_ONLY
/**
 *	@brief	Init the period of the PWM Channels
 *	@param	period	PWM period
 *	@remarks	PWM_Mapping
 *	<div class="PWM_Mapping">
 *	<table><tr>
 *	<th>LPC2103</th>
 *	<td>PWM_InitPeriod2103()</td></tr>
 *	<th>LPC213x</th>
 *	<td>PWM_InitPeriod213x()</td></tr>
 *	</table></div>
 * */
void PWM_InitPeriod(int period);
/**
 *	@brief	Init the PWM channel
 *
 *	Perform pin select and PWM output enable.
 *	Set the PWM value specified
 *
 *	@param	channel	PWM channel to be inited
 *	@param	level	PWM value
 *
 *	@remarks	PWM_Mapping
 *	<div class="PWM_Mapping">
 *	<table><tr>
 *	<th>LPC2103</th>
 *	<td>PWM_InitChannel2103()</td></tr>
 *	<th>LPC213x</th>
 *	<td>PWM_InitChannel213x()</td></tr>
 *	</table></div>
 * */
void PWM_InitChannel(int channel, int level);
/**
 *	@brief	Set PWM value
 *
 *	@param	channel	PWM channel to be set
 *	@param	level	PWM value
 *	@remarks	PWM_Mapping
 *	<div class="PWM_Mapping">
 *	<table><tr>
 *	<th>LPC2103</th>
 *	<td>PWM_Set2103()</td></tr>
 *	<th>LPC213x</th>
 *	<td>PWM_Set213x()</td></tr>
 *	</table></div>
 * */
void PWM_Set(int channel, int level);
void PWM_InitPeriod213x(int period);
void PWM_Set213x(int channel, int level);
void PWM_InitChannel213x(int channel, int level);
void PWM_InitPeriod2103(int period);
void PWM_Set2103(int channel, int level);
void PWM_InitChannel2103(int channel, int level);
#else
#ifdef	__LPC213x_H	
	/**
 	*
	*	 LPC 213x
	*
	* */
	#define PWM_InitPeriod	PWM_InitPeriod213x
	#define	PWM_InitChannel	PWM_InitChannel213x
	#define	PWM_Set			PWM_Set213x
	void PWM_InitPeriod213x(int period);
	void PWM_Set213x(int channel, int level);
	void PWM_InitChannel213x(int channel, int level);
#else
	/**
	 *
	 *	 LPC 2103
	 *
	 * */
	#define PWM_InitPeriod	PWM_InitPeriod2103
	#define	PWM_InitChannel	PWM_InitChannel2103
	#define	PWM_Set			PWM_Set2103
	void PWM_InitPeriod2103(int period);
	void PWM_Set2103(int channel, int level);
	void PWM_InitChannel2103(int channel, int level);
#endif
#endif

int PWM_ReturnPeriod(void);
void PWM_InitFrequency(int frequency);
void PWM_SetPercentage(int channel,float percentage);
#endif
