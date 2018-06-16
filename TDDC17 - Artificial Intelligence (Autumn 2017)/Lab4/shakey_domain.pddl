(define (domain shakey-problem)

    (:requirements
        :strips
        :equality
        :typing
        :adl)

    (:types
        

        ;There are 3 rooms
        box         ;There is one large box
        door        ;There are 3 doors
        shakey      ;There is only one SHAKEY BBABEYEEYYEYE
        object      ;There are several blocks
        position    ;Things have postions
        room        ;Roomy

    )

    (:predicates
        (onTopBox   ?s - shakey)                ;Shakey can be ontop of boxes
        (holdRight  ?o - object)                ;Shakey can carry objects in right hand
        (holdLeft   ?o - object)                ;Shakey can carry object in left hand
        (lightPos   ?r - room)                  ;Light can be on for diffrent rooms
        (isLit      ?r - room)
        (boxPos     ?b - box ?r - room)         ;Box position
        (objectPos  ?o - object ?r - room)        ;Object position
        (shakeyPos  ?s - shakey ?r - room)        ;Shakey position.
        (wideDoor   ?r1 ?r2 - room)             ;Doors can be narrow or wide
        (adjacent   ?r1 ?r2 - room)             ;Is shakey adjacent with p.
        
    )


    
    (:action move ;Movement when not holding/pushing for Shakey.
        :parameters     (?s - shakey    ?from ?to - room)
        :precondition   (and 
                            (or     
                                (adjacent ?from ?to)
                                (adjacent ?to ?from)
                )
            (shakeyPos ?s ?from)                        
            )
                        
        :effect         (and
                            (not (shakeyPos ?s ?from))
                            (shakeyPos ?s ?to)
                        )
    )


    (:action push ;Shakey pushing a box from ?r1 to ?r2 position.
        :parameters      (?b - box   ?r1 ?r2 - room    ?s - shakey)
        :precondition   (and
                                (or 
                                    (wideDoor ?r1 ?r2)
                                    (wideDoor ?r2 ?r1)
                                )
                            (boxPos ?b ?r1)
                            (not (boxPos ?b ?r2))
                            (adjacent ?r1 ?r2)
                            (shakeyPos ?s ?r1)
                        )
        :effect         (and
                            (boxPos ?b ?r2 )
                            (not (boxPos ?b ?r1))
                            (shakeyPos ?s ?r2)
                            (not (shakeyPos ?s ?r1))
                        )
    )


    (:action pickUpLeft ;Shakey pick up small object with left grabber.
        :parameters     (?s - shakey    ?o - object     ?r - room) 
        :precondition   (and
                            (not (holdLeft ?o))
                            (shakeyPos ?s ?r)
                            (objectPos ?o ?r)
                            (isLit ?r)
                        )
        :effect         (and
                            (holdLeft ?o)
                        )
    )

    (:action pickUpRight ;Shakey pick up small object with right grabber.
        :parameters     (?o - object     ?r - room     ?s - shakey) 
        :precondition   (and
                            (not (holdRight ?o))
                            (shakeyPos ?s ?r)
                            (objectPos ?o ?r)
                            (isLit ?r)
                        )
        :effect         (and
                            (holdRight ?o)
                        )
    )

    (:action dropLeft ;Shakey drop object with left grabber.
        :parameters     (?o - object     ?r - room)
        :precondition   (holdLeft ?o)
        :effect         (and
                            (not (holdLeft ?o))
                            (objectPos ?o ?r)
                        )
    )

    (:action dropRight ;Shakey drop object with right grabber.
        :parameters     (?o - object    ?r - room)
        :precondition   (holdRight ?o)
        :effect         (and
                            (not (holdRight ?o))
                            (objectPos ?o ?r)
                        )
    )

    (:action climbOnBox ;Shakey climb on top of box.
        :parameters     (?s - shakey    ?r - room    ?o - object)
        :precondition   (and
                            (not (OnTopBox ?s))
                            (shakeyPos ?s ?r)
                            (not (holdRight ?o))
                            (not (holdLeft ?o))
                        )
        :effect         (OnTopBox ?s)
    )

    (:action turnLightOn ;Shakey turns on the light in the room.
        :parameters     (?s - shakey   ?b - object     ?r - room)
        :precondition   (and
                            (lightPos ?r)
                            (OnTopBox ?s)
                            (not (isLit ?r))
                        )
        :effect         (isLit ?r)
    )

    (:action turnLightOff ;Shakey turns off the light in the room.
        :parameters     (?s - shakey    ?b - object     ?r - room)
        :precondition   (and
                            (lightPos ?r)
                            (OnTopBox ?s)
                            (isLit ?r)
                        )
        :effect         (not (isLit ?r))
    )
)