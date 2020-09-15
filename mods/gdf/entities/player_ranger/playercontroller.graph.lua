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
      16,
      2,
    },
    {
      1,
      3,
      38,
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
      21,
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
      37,
      2,
    },
    {
      1,
      3,
      18,
      1,
    },
    {
      1,
      3,
      29,
      2,
    },
    {
      1,
      3,
      13,
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
      17,
      1,
    },
    {
      3,
      3,
      13,
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
      14,
      1,
      38,
      1,
    },
    {
      15,
      1,
      25,
      2,
    },
    {
      16,
      1,
      22,
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
      32,
      1,
    },
    {
      18,
      1,
      33,
      1,
    },
    {
      19,
      1,
      38,
      3,
    },
    {
      20,
      1,
      37,
      3,
    },
    {
      21,
      1,
      25,
      1,
    },
    {
      23,
      1,
      16,
      3,
    },
    {
      24,
      1,
      22,
      4,
    },
    {
      25,
      1,
      17,
      2,
    },
    {
      26,
      1,
      22,
      3,
    },
    {
      27,
      1,
      34,
      2,
    },
    {
      28,
      1,
      37,
      1,
    },
    {
      29,
      1,
      30,
      1,
    },
    {
      30,
      1,
      14,
      2,
    },
    {
      31,
      1,
      29,
      1,
    },
    {
      32,
      1,
      20,
      2,
    },
    {
      32,
      2,
      20,
      1,
    },
    {
      33,
      1,
      34,
      1,
    },
    {
      34,
      1,
      28,
      2,
    },
    {
      34,
      1,
      12,
      3,
    },
    {
      35,
      1,
      30,
      2,
    },
    {
      36,
      1,
      21,
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
        4,
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
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              10,
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
            {
              10,
              1,
              2,
              2,
            },
            {
              11,
              1,
              10,
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
                      5,
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
                      2,
                    },
                    {
                      4,
                      1,
                      5,
                      2,
                    },
                    {
                      5,
                      1,
                      6,
                      1,
                    },
                    {
                      7,
                      1,
                      6,
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
                        'idle',
                      },
                      name = 'string',
                    },
                    {
                      name = 'script/getcontext',
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
                      5,
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
                      5,
                      2,
                    },
                    {
                      6,
                      1,
                      5,
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
                        'walk',
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
                3,
                {
                  links = {
                    {
                      2,
                      1,
                      3,
                      1,
                    },
                    {
                      3,
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
                          'Following Path',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/isfollowingpath',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'moving',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                4,
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
                      4,
                      1,
                    },
                    {
                      3,
                      1,
                      5,
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
                      1,
                      1,
                    },
                    {
                      6,
                      1,
                      5,
                      2,
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
                      name = 'entity/isfollowingpath',
                    },
                    {
                      name = 'math/or',
                    },
                    {
                      name = 'math/not',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'not moving',
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
                      3,
                      1,
                      2,
                      3,
                    },
                    {
                      4,
                      1,
                      2,
                      2,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
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
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'init animation',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'flow/cooldown',
    },
    {
      name = 'script/condition',
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
        'atan(y,x)',
      },
      name = 'math/formula',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        'missile3',
      },
      name = 'string',
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
    {
      name = 'controller/axisvalue',
    },
    {
      name = 'math/greaterthan',
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
      name = 'math/vector2/vector2length',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'TRIGGERRIGHT',
      },
      name = 'string',
    },
    {
      name = 'entity/setheading',
    },
    {
      name = 'entity/jump',
    },
  },
}