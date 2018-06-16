public class StateAndReward {

	private static final double angle_constraint = Math.PI/2;
	private static final int nrStates = 4;
	/* State discretization function for the angle controller */
	public static String getStateAngle(double angle, double vx, double vy) {
		
		/* TODO: IMPLEMENT THIS FUNCTION */
		int discretizeValue = discretize2(angle,nrStates,-angle_constraint, angle_constraint);
		String state = Integer.toString(discretizeValue);

		return state;
	}

	/* Reward function for the angle controller */
	public static double getRewardAngle(double angle, double vx, double vy) {
		/* TODO: IMPLEMENT THIS FUNCTION */
		//double reward = 
		//double reward = 1- ((Math.PI/2 - Math.abs(angle))/Math.PI/2);
		double reward = 1 - (Math.abs(angle)/Math.PI);
		reward = Math.pow(reward,2);
		return reward;
	}

	/* State discretization function for the full hover controller */
	public static String getStateHover(double angle, double vx, double vy) {
		
		int discy = discretize(vy, 4, -0.5, 0.5);
		int discx = discretize(vx, 4, -1.0, 1.0);
		
		String state = Integer.toString((Integer.parseInt((getStateAngle(angle, vx, vy)) + discy + discx)));
		
		return state;
	}

	/* Reward function for the full hover controller */
	public static double getRewardHover(double angle, double vx, double vy) {

		/* TODO: IMPLEMENT THIS FUNCTION */
		double vyRew = 0.0;
		double vxRew = 0.0;
		
		if(Math.abs(vy) < 0.5)	
			vyRew = 0.5 - Math.abs(vy);
		
		if(Math.abs(vx) < 1.0)
			vxRew = (0.5  - (Math.abs(vx)/2.0));
			
		double reward = 1.5*getRewardAngle(angle, vx, vy) + 1.5*vyRew + vxRew;

		return reward;
	}

	// ///////////////////////////////////////////////////////////
	// discretize() performs a uniform discretization of the
	// value parameter.
	// It returns an integer between 0 and nrValues-1.
	// The min and max parameters are used to specify the interval
	// for the discretization.
	// If the value is lower than min, 0 is returned
	// If the value is higher than min, nrValues-1 is returned
	// otherwise a value between 1 and nrValues-2 is returned.
	//
	// Use discretize2() if you want a discretization method that does
	// not handle values lower than min and higher than max.
	// ///////////////////////////////////////////////////////////
	public static int discretize(double value, int nrValues, double min, double max) {
		if (nrValues < 2) {
			return 0;
		}

		double diff = max - min;

		if (value < min) {
			return 0;
		}
		if (value > max) {
			return nrValues - 1;
		}

		double tempValue = value - min;
		double ratio = tempValue / diff;

		return (int) (ratio * (nrValues - 2)) + 1;
	}

	// ///////////////////////////////////////////////////////////
	// discretize2() performs a uniform discretization of the
	// value parameter.
	// It returns an integer between 0 and nrValues-1.
	// The min and max parameters are used to specify the interval
	// for the discretization.
	// If the value is lower than min, 0 is returned
	// If the value is higher than max, nrValues-1 is returned
	// otherwise a value between 0 and nrValues-1 is returned.
	// ///////////////////////////////////////////////////////////
	public static int discretize2(double value, int nrValues, double min,
			double max) {
		double diff = max - min;

		if (value < min) {
			return 0;
		}
		if (value > max) {
			return nrValues - 1;
		}

		double tempValue = value - min;
		double ratio = tempValue / diff;

		return (int) (ratio * nrValues);
	}

}
