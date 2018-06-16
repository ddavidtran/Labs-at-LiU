(define (problem shakeyshake)
  (:domain shakey-problem)
  (:objects 
	s			        -	shakey
	b			        -	box
	r1 r2 r3		    -	room

)
  (:init 
	(shakeyPos s r1) (lightPos r1) (lightPos r2) (lightPos r3)
	(wideDoor r1 r2) (wideDoor r2 r3) (boxPos b r1) (adjacent r1 r2) (adjacent r2 r3)


)
  (:goal
    (and
	    (isLit r1) (isLit r2) (isLit r3)
	)
  )
)