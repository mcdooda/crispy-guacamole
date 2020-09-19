return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      3,
      1,
    },
    {
      1,
      3,
      4,
      1,
    },
    {
      1,
      3,
      5,
      1,
    },
    {
      1,
      3,
      9,
      2,
    },
    {
      1,
      3,
      11,
      2,
    },
    {
      1,
      3,
      22,
      2,
    },
    {
      1,
      3,
      16,
      2,
    },
    {
      1,
      3,
      15,
      2,
    },
    {
      1,
      3,
      12,
      2,
    },
    {
      1,
      3,
      23,
      1,
    },
    {
      1,
      3,
      20,
      2,
    },
    {
      3,
      1,
      11,
      1,
    },
    {
      3,
      2,
      15,
      1,
    },
    {
      4,
      1,
      8,
      1,
    },
    {
      5,
      1,
      6,
      1,
    },
    {
      6,
      1,
      7,
      1,
    },
    {
      6,
      2,
      7,
      2,
    },
    {
      7,
      1,
      8,
      2,
    },
    {
      8,
      1,
      9,
      3,
    },
    {
      9,
      1,
      14,
      1,
    },
    {
      9,
      1,
      12,
      1,
    },
    {
      10,
      1,
      9,
      4,
    },
    {
      11,
      1,
      9,
      1,
    },
    {
      12,
      1,
      28,
      1,
    },
    {
      13,
      1,
      28,
      2,
    },
    {
      13,
      1,
      12,
      3,
    },
    {
      14,
      1,
      22,
      1,
    },
    {
      16,
      1,
      19,
      1,
    },
    {
      17,
      1,
      16,
      1,
    },
    {
      18,
      1,
      26,
      2,
    },
    {
      18,
      2,
      26,
      1,
    },
    {
      19,
      1,
      14,
      2,
    },
    {
      21,
      1,
      13,
      1,
    },
    {
      23,
      1,
      18,
      1,
    },
    {
      23,
      1,
      21,
      1,
    },
    {
      24,
      1,
      22,
      3,
    },
    {
      25,
      1,
      19,
      2,
    },
    {
      26,
      1,
      20,
      3,
    },
    {
      27,
      1,
      13,
      2,
    },
    {
      28,
      1,
      20,
      1,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        5,
      },
      name = 'script/impulseall',
    },
    {
      name = 'controller/leftstickdirection',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'math/vector3/vector3split',
    },
    {
      name = 'vector2',
    },
    {
      name = 'math/vector2/vector2add',
    },
    {
      name = 'entity/moveto',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'entity/clearpath',
    },
    {
      name = 'entity/setstrafing',
    },
    {
      name = 'math/greaterthan',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              2,
              2,
            },
            {
              2,
              2,
              6,
              2,
            },
            {
              3,
              1,
              2,
              1,
            },
            {
              4,
              2,
              8,
              2,
            },
            {
              5,
              1,
              4,
              1,
            },
            {
              6,
              1,
              4,
              2,
            },
            {
              7,
              1,
              6,
              1,
            },
            {
              8,
              1,
              2,
              3,
            },
            {
              9,
              1,
              8,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                3,
                1,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      1,
                    },
                    {
                      2,
                      1,
                      4,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      2,
                    },
                    {
                      4,
                      1,
                      6,
                      1,
                    },
                    {
                      5,
                      1,
                      4,
                      3,
                    },
                    {
                      6,
                      1,
                      7,
                      1,
                    },
                    {
                      8,
                      1,
                      7,
                      3,
                    },
                    {
                      9,
                      1,
                      4,
                      2,
                    },
                    {
                      9,
                      1,
                      6,
                      2,
                    },
                    {
                      9,
                      1,
                      7,
                      2,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'print',
                    },
                    {
                      loadArguments = {
                        'idle',
                      },
                      name = 'string',
                    },
                    {
                      name = 'entity/setcycleanimation',
                    },
                    {
                      loadArguments = {
                        'move',
                      },
                      name = 'string',
                    },
                    {
                      name = 'entity/resetcycleanimation',
                    },
                    {
                      name = 'entity/setcycleanimated',
                    },
                    {
                      loadArguments = {
                        false,
                      },
                      name = 'boolean',
                    },
                    {
                      name = 'script/getcontext',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'idle',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                2,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      1,
                    },
                    {
                      2,
                      1,
                      4,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      2,
                    },
                    {
                      4,
                      1,
                      7,
                      1,
                    },
                    {
                      5,
                      1,
                      4,
                      3,
                    },
                    {
                      6,
                      1,
                      4,
                      2,
                    },
                    {
                      6,
                      1,
                      7,
                      2,
                    },
                    {
                      8,
                      1,
                      7,
                      3,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'print',
                    },
                    {
                      loadArguments = {
                        'move',
                      },
                      name = 'string',
                    },
                    {
                      name = 'entity/setcycleanimation',
                    },
                    {
                      loadArguments = {
                        'move',
                      },
                      name = 'string',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/setcycleanimated',
                    },
                    {
                      loadArguments = {
                        true,
                      },
                      name = 'boolean',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'walk',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                6,
                {
                  links = {
                    {
                      2,
                      1,
                      3,
                      1,
                    },
                    {
                      2,
                      1,
                      5,
                      1,
                    },
                    {
                      3,
                      1,
                      4,
                      1,
                    },
                    {
                      4,
                      1,
                      6,
                      1,
                    },
                    {
                      5,
                      1,
                      6,
                      2,
                    },
                    {
                      6,
                      1,
                      1,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      loadArguments = {
                        {
                          1,
                          'Result',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/ismidair',
                    },
                    {
                      name = 'math/not',
                    },
                    {
                      name = 'entity/isfollowingpath',
                    },
                    {
                      name = 'math/and',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'ground + moving',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                5,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      1,
                    },
                    {
                      1,
                      1,
                      4,
                      1,
                    },
                    {
                      2,
                      1,
                      3,
                      1,
                    },
                    {
                      3,
                      1,
                      7,
                      1,
                    },
                    {
                      4,
                      1,
                      5,
                      1,
                    },
                    {
                      5,
                      1,
                      7,
                      2,
                    },
                    {
                      7,
                      1,
                      6,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/ismidair',
                    },
                    {
                      name = 'math/not',
                    },
                    {
                      name = 'entity/isfollowingpath',
                    },
                    {
                      name = 'math/not',
                    },
                    {
                      loadArguments = {
                        {
                          1,
                          'Result',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                    {
                      name = 'math/and',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'ground + not moving',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      loadArguments = {
        'TRIGGERLEFT',
      },
      name = 'string',
    },
    {
      name = 'math/vector2/vector2split',
    },
    {
      name = 'math/greaterthan',
    },
    {
      name = 'entity/setheading',
    },
    {
      name = 'math/vector2/vector2length',
    },
    {
      name = 'entity/jump',
    },
    {
      name = 'controller/rightstickdirection',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        0.20000000298023,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'atan(y,x)',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'script/condition',
    },
  },
}