#ifndef SECTION_H_INCLUDED
#define SECTION_H_INCLUDED
#include <utility>
#include "GCodeCommand.h"
#include "../genmath/LongDouble.h"
//G0 FIRST PIONT
//LALR1
// 1X INC-DEC AVG LIN


namespace printersimulation {

	struct Section {

		static genmath::LongDouble XJerk;// x axial jerk
		static genmath::LongDouble YJerk;// y axial jerk
		static genmath::LongDouble XAcc;// axial acceleration of x
		static genmath::LongDouble YAcc;// axial acceleration of y
		static genmath::LongDouble MinResSpd;// lower limit of resultant speed
		static genmath::LongDouble MaxResSpd;// upper limit of resultant speed
		static genmath::LongDouble MaxResAcc;// upper limit of resultant acceleration
		static genmath::LongDouble MaxResJerk;// upper limit of resultant jerk
		static genmath::LongDouble MaxResSpdTime;// time to reach speed limit from 0
		static genmath::LongDouble MaxResSpdLen;// length has taken from 0 until speed reaches its upper limit
		static genmath::LongDouble Zero;
		static genmath::LongDouble One;
		static genmath::LongDouble Two;
		static genmath::LongDouble Sixty;

		static void Init();
		static void UpdateGlobalResSpd(genmath::LongDouble new_res_spd);

		Section();
		Section(GCodeCommand<genmath::LongDouble>* cmd_param);
		Section(const Section& orig);
		virtual ~Section();

		Section& operator=(const Section& orig);
		void UpdateSection(GCodeCommand<genmath::LongDouble>* cmd_param, genmath::LongDouble& unit_time_step);
		void SetPrevSect(Section* prev_sect);
		std::pair<genmath::LongDouble, genmath::LongDouble> operator()(genmath::LongDouble t);
		void ConfigureSpeed(genmath::LongDouble& spd_mul);


		GCodeCommand<genmath::LongDouble>* start_point_;
		GCodeCommand<genmath::LongDouble>* end_point_;
		genmath::LongDouble start_x_;
		genmath::LongDouble start_y_;
		genmath::LongDouble end_x_;
		genmath::LongDouble end_y_;
		genmath::LongDouble x_dir_;
		genmath::LongDouble y_dir_;
		genmath::LongDouble ratio_x_;
		genmath::LongDouble ratio_y_;
		genmath::LongDouble start_time_;
		genmath::LongDouble end_time_;
		genmath::LongDouble len_;
		genmath::LongDouble fst_len_;// length of first subsection
		genmath::LongDouble snd_len_;// length of second subsection
		genmath::LongDouble avg_spd_;// overall average speed
		genmath::LongDouble fst_avg_spd_;// average speed of first subsection
		genmath::LongDouble snd_avg_spd_;// average speed of second subsection
		Section* prev_sect_;
	};
}

#endif// SECTION_H_INCLUDED